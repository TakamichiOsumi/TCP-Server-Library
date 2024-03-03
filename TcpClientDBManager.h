#ifndef __TCP_CLIENT_DB_MANAGER__
#define __TCP_CLIENT_DB_MANAGER__

#include <stdint.h>
#include <stdlib.h>
#include "Linked-List/linked_list.h"
#include "TcpServerController.h"

typedef struct TcpClientDBManager {
    TcpServerController *tsc;
    linked_list *tcp_client_db;
} TcpClientDBManager;

TcpClientDBManager *DBM_create(TcpServerController *tsc);
void DBM_init_client_db_manager(TcpClientDBManager *dbm);
void DBM_destroy(TcpClientDBManager *dbm);

#endif
