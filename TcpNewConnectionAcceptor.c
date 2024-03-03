#include <stdlib.h>
#include <stdio.h>
#include "TcpNewConnectionAcceptor.h"

TcpNewConnectionAcceptor *
CAS_create(TcpServerController *tsc){
    TcpNewConnectionAcceptor *acceptor;

    if ((acceptor = (TcpNewConnectionAcceptor *)
	 malloc(sizeof(TcpNewConnectionAcceptor))) == NULL){
	perror("malloc");
	exit(-1);
    }

    acceptor->tsc = tsc;

    return acceptor;
}

void
CAS_start_acceptor_thread(TcpNewConnectionAcceptor *cas){
}

void
CAS_destroy(TcpNewConnectionAcceptor *cas){
}
