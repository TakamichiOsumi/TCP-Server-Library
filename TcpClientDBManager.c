#include <stdio.h>
#include <stdlib.h>
#include "TcpClientDBManager.h"

static bool
search_tcp_client_by_id(void *p, void *key){
    return false;
}

TcpClientDBManager *
DBM_create(TcpServerController *tsc){
    TcpClientDBManager *db_manager;

    if ((db_manager = (TcpClientDBManager *)
	 malloc(sizeof(TcpClientDBManager))) == NULL){
	perror("malloc");
	exit(-1);
    }

    db_manager->tsc = tsc;
    db_manager->tcp_client_db = ll_init(search_tcp_client_by_id);

    return db_manager;
}

void
DBM_init_client_db_manager(TcpClientDBManager *dbm){
}

void
DBM_destroy(TcpClientDBManager *dbm){
}
