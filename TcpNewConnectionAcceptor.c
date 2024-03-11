#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "TcpNewConnectionAcceptor.h"
#include "TcpClient.h"

/*
 * New connections are accepted using 'accept()' sys call.
 *
 * Create an master socket file descriptor using socket().
 */
static int master_socket;

/* Thread to listen for new connection from new client */
static pthread_t *conn_acceptor;

TcpNewConnectionAcceptor *
CAS_create(TcpServerController *tsc){
    TcpNewConnectionAcceptor *acceptor;

    if ((acceptor = (TcpNewConnectionAcceptor *)
	 malloc(sizeof(TcpNewConnectionAcceptor))) == NULL){
	perror("malloc");
	exit(-1);
    }

    /* The socket setup will be done in CAS_listen_new_tcp_connection() */
    if ((master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
	perror("socket");
	exit(-1);
    }

    if ((conn_acceptor = (pthread_t *) malloc(sizeof(pthread_t))) == NULL){
	perror("malloc");
	exit(-1);
    }

    acceptor->tsc = tsc;

    return acceptor;
}

static void *
CAS_listen_new_tcp_connection(void *arg){
    TcpNewConnectionAcceptor *cas = (TcpNewConnectionAcceptor *) arg;
    /* For server bind */
    struct sockaddr_in tcp_server_addr;
    /* For connected client socket */
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = (sizeof(client_addr));
    int comm_socket_fd;

    /* Name the socket created in CAS_create */
    memset(&tcp_server_addr, 0, sizeof(tcp_server_addr));
    /* IPv4 */
    tcp_server_addr.sin_family = AF_INET;
    /* Convert the port_no to network byte order */
    tcp_server_addr.sin_port = htons(cas->tsc->port_no);
    /* cas->tsc->ip_addr is already inet_addr()ed */
    tcp_server_addr.sin_addr.s_addr = cas->tsc->ip_addr;

    printf("debug : %s will bind the socket with %s:%u\n",
	   __FUNCTION__, inet_ntoa(tcp_server_addr.sin_addr),
	   cas->tsc->port_no);

    if (bind(master_socket,
	     (struct sockaddr *) &tcp_server_addr,
	     sizeof(tcp_server_addr)) == -1){
	perror("bind");
	exit(-1);
    }

    /* Create a queue */
    if (listen(master_socket, 5) < 0){
	perror("listen");
	exit(-1);
    }

    /*
     * Now, client can request new connection.
     */

    while(1){

	printf("debug : will call accept()\n");

	comm_socket_fd = accept(master_socket,
				(struct sockaddr *) &client_addr,
				&client_addr_len);
	if (comm_socket_fd < 0){
	    fprintf(stderr,
		    "debug (%s): new connection failed\n", __FUNCTION__);
	    continue;
	}else{
	    TcpClient *new_client;

	    fprintf(stderr, "debug (%s): new connection '%s.%d' accepted\n",
		    __FUNCTION__,
		    inet_ntoa(client_addr.sin_addr),
		    client_addr.sin_port);

	    new_client = TcpClient_create(comm_socket_fd,
					  client_addr.sin_addr.s_addr,
					  client_addr.sin_port,
					  cas->tsc);

	    if (cas->tsc->connected_cb)
		cas->tsc->connected_cb(cas->tsc, new_client);

	    TSC_process_new_client(new_client);
	}
    }

    close(master_socket);

    return NULL;
}

/*
 * Launch a thread to handle a client connection.
 */
void
CAS_start_acceptor_thread(TcpNewConnectionAcceptor *cas){
    if (pthread_create(conn_acceptor, NULL,
		       CAS_listen_new_tcp_connection, (void *) cas) != 0){
	perror("pthread_create");
	exit(-1);
    }

    printf("debug : '%s' has created a thread to accept new connections\n",
	   __FUNCTION__);
}

void
CAS_destroy(TcpNewConnectionAcceptor *cas){
    if (cas == NULL)
	return;

    free(conn_acceptor);
    free(cas);
}
