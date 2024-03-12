#include <assert.h>
#include <memory.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "TcpClientServiceManager.h"
#include "Linked-List/linked_list.h"

/* buffer to receive message from client */
#define TCP_CLIENT_MSG_BUFFER_SIZE 1024
static char tcp_client_message_buffer[TCP_CLIENT_MSG_BUFFER_SIZE];

/* Local copy of TcpClientDBManager's tcp_client_db */
static linked_list *copied_tcp_client_db;

static bool
DRS_search_tcp_client_by_key(void *p, void *key){
    TcpClient *tcp_client = (TcpClient *) p;

    if (tcp_client->comm_fd == (uintptr_t) key)
	return true;
    else
	return false;
}

/* No dynamically-allocated unique member in TcpClient */
static void
DRS_free_tcp_client(void *data){}

static int
DRS_get_max_comm_fd(void){
    TcpClient *tcp_client;
    node *n;
    int max_comm_fd = 0;

    ll_begin_iter(copied_tcp_client_db);
    while((n = ll_get_iter_node(copied_tcp_client_db)) != NULL){
	tcp_client = (TcpClient *) n->data;
	if (tcp_client->comm_fd > max_comm_fd)
	    max_comm_fd = tcp_client->comm_fd;
    }
    ll_end_iter(copied_tcp_client_db);

    return max_comm_fd;
}

TcpClientServiceManager *
DRS_create(TcpServerController *tsc){
    TcpClientServiceManager *service_manager;

    if ((service_manager = (TcpClientServiceManager *)
	 malloc(sizeof(TcpClientServiceManager))) == NULL){
	perror("malloc");
	exit(-1);
    }

    /* Init local copy of client db */
    copied_tcp_client_db = ll_init(DRS_search_tcp_client_by_key,
				   DRS_free_tcp_client);

    service_manager->max_fd = 0;
    FD_ZERO(&service_manager->active_fd_set);
    FD_ZERO(&service_manager->backup_fd_set);

    if ((service_manager->service_thread = (pthread_t *)
	 malloc(sizeof(pthread_t))) == NULL){
	perror("malloc");
	exit(-1);
    }

    service_manager->tsc = tsc;

    return service_manager;
}

static void
DRS_copy_client_fds_to_fd_set(fd_set *set){
    TcpClient *tcp_client;
    node *n;

    ll_begin_iter(copied_tcp_client_db);
    while((n = ll_get_iter_node(copied_tcp_client_db)) != NULL){
	tcp_client = (TcpClient *) n->data;
	FD_SET(tcp_client->comm_fd, set);
    }
    ll_end_iter(copied_tcp_client_db);
}

void
DRS_delete_disconnected_client(TcpClient *tcp_client){
    assert(ll_remove(copied_tcp_client_db, (void *) tcp_client->comm_fd) != NULL);
}

static void*
DRS_process_comm_fds(void *arg){
    TcpClientServiceManager *drs = (TcpClientServiceManager *) arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int recv_bytes;
    TcpClient *tcp_client;
    node *n;

    /* Enable the cancellation in deferred mode */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_DEFERRED, NULL);

    drs->max_fd = DRS_get_max_comm_fd();

    while(1){

	FD_ZERO(&drs->backup_fd_set);
	DRS_copy_client_fds_to_fd_set(&drs->backup_fd_set);
	memcpy(&drs->active_fd_set, &drs->backup_fd_set, sizeof(fd_set));

	printf("debug : will be blocked on select in %s\n", __FUNCTION__);

	select(drs->max_fd + 1, &drs->active_fd_set, 0, 0, 0);

	ll_begin_iter(copied_tcp_client_db);

	while((n = ll_get_iter_node(copied_tcp_client_db)) != NULL){
	    tcp_client = (TcpClient *) n->data;

	    if (FD_ISSET(tcp_client->comm_fd, &drs->active_fd_set)){

		printf("debug : found the TcpClient who owns '%lu' comm fd \n",
		       tcp_client->comm_fd);

		recv_bytes = recvfrom(tcp_client->comm_fd,
				      tcp_client_message_buffer,
				      TCP_CLIENT_MSG_BUFFER_SIZE,
				      0,
				      (struct sockaddr *) &client_addr,
				      &addr_len);
		if (recv_bytes < 0){
		    perror("recvfrom");
		    exit(-1);
		}else if (recv_bytes == 0){
		    /*
		     * Now the client gets disconnected.
		     *
		     * Remove the client object from both of 'TcpClientDBManager'
		     * database and its local copy in 'copied_tcp_client_db'.
		     *
		     * Lastly, free the TcpClient object itself.
		     */
		    TSC_delete_disconnected_client(tcp_client);
		    DRS_delete_disconnected_client(tcp_client);
		    free(tcp_client);
		    printf("debug : deleted all references to tcp client by %s\n",
			   __FUNCTION__);
		}else{
		    if (drs->tsc->received_msg_cb){
			drs->tsc->received_msg_cb(drs->tsc,
						  tcp_client,
						  tcp_client_message_buffer,
						  TCP_CLIENT_MSG_BUFFER_SIZE);
		    }
		}
		break;
	    } /* FD_ISSET */
	}

	ll_end_iter(copied_tcp_client_db);
    }

    return NULL;
}

void
DRS_start_manager_thread(TcpClientServiceManager *drs){
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    if (pthread_create(drs->service_thread,
		       &attr,
		       DRS_process_comm_fds,
		       (void *) drs) != 0){
	perror("pthread_create");
	exit(-1);
    }
}

void
DRS_stop_manager_thread(TcpClientServiceManager *drs){
    /* Send a cancellation signal to the service thread */
    pthread_cancel(*drs->service_thread);
    /*
     * Get blocked until the cancellation is successful.
     * Wait for the thread joined back.
     */
    pthread_join(*drs->service_thread, NULL);

    /* Now, can guarantee that the service thread is canceled */

    /* Clean up */
    free(drs->service_thread);
    drs->service_thread = NULL;
}

/*
 * This function cancels the already-launched service manager thread.
 *
 * The purpose of this function is to update communication file descriptors
 * for select arguments, even when the thread is in wait state for select.
  */
void
DRS_start_listen_comm_fd(TcpClientServiceManager *drs,
			 TcpClient *tcp_client){

    DRS_stop_manager_thread(drs);
    printf("debug : the service manager thread is cancelled\n");

    ll_insert(copied_tcp_client_db, (void *) tcp_client);
    printf("debug : the new client was added to the local copy of DRS also\n");

    if ((drs->service_thread = (pthread_t *) malloc(sizeof(pthread_t))) == NULL){
	perror("malloc");
	exit(-1);
    }

    DRS_start_manager_thread(drs);
}

void
DRS_destroy(TcpClientServiceManager *drs){
}
