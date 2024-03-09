#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "TcpNewConnectionAcceptor.h"

/*
 * New connections are accepted using 'accept()' sys call.
 *
 * Create an master socket file descriptor using socket().
 */
static int master_fd;

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

    if ((master_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
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
    TcpNewConnectionAcceptor *cas = arg;


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

    close(master_fd);
    free(conn_acceptor);
    free(acceptor);
    free(cas);
}
