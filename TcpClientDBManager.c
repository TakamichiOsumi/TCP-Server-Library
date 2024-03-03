#include <stdio.h>
#include <stdlib.h>
#include "TcpClientDBManager.h"

TcpClientDBManager *
DBM_create(TcpServerController *tsc){
    TcpClientDBManager *db_manager;

    if ((db_manager = (TcpClientDBManager *)
	 malloc(sizeof(TcpClientDBManager))) == NULL){
	perror("malloc");
	exit(-1);
    }

    db_manager->tsc = tsc;

    return db_manager;
}

void
DBM_destroy(TcpClientDBManager *dbm){
}
