#include <stdio.h>
#include <stdlib.h>
#include "TcpClientDBManager.h"

static bool
search_tcp_client_by_id(void *p, void *key){
    return false;
}

TcpClient *
DBM_store_client_conn(int comm_fd, uint32_t ip_addr, uint16_t port_no,
		      TcpServerController *tsc){
    TcpClient *client;

    if ((client = (TcpClient *) malloc(sizeof(TcpClient))) == NULL){
	perror("malloc");
	exit(-1);
    }

    client->comm_fd = comm_fd;
    client->ip_addr = ip_addr;
    client->port_no = port_no;
    client->tsc = tsc;

    return NULL;
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
