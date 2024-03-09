#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "TcpNewConnectionAcceptor.h"

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
    tcp_server_addr.sin_family = AF_INET; /* IPv4 */
    tcp_server_addr.sin_port = htons(cas->tsc->port_no);
    tcp_server_addr.sin_addr.s_addr = htonl(cas->tsc->ip_addr);

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
	comm_socket_fd = accept(master_socket,
				(struct sockaddr *) &client_addr,
				&client_addr_len);
	fprintf(stderr, "debug : new connection accepted\n");
    }

    close(master_socket);

    return NULL;
}

/*
 * Invoke accept() to accept new connections in the loop and
 * notify the application for new connections.
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
