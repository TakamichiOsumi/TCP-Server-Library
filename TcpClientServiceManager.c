
#include <stdio.h>
#include <stdlib.h>
#include "TcpClientServiceManager.h"

TcpClientServiceManager *
DRS_create(TcpServerController *tsc){
    TcpClientServiceManager *service_manager;

    if ((service_manager = (TcpClientServiceManager *)
	 malloc(sizeof(TcpClientServiceManager))) == NULL){
	perror("malloc");
	exit(-1);
    }

    return service_manager;
}

void
DRS_destroy(TcpClientServiceManager *drs){
}
