#ifndef __TCP_CLIENT_DB_MANAGER__
#define __TCP_CLIENT_DB_MANAGER__

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "Linked-List/linked_list.h"
#include "TcpServerController.h"
#include "TcpClient.h"

typedef struct TcpClientDBManager {
    TcpServerController *tsc;
    linked_list *tcp_client_db;
    pthread_rwlock_t rwlock;
} TcpClientDBManager;

TcpClientDBManager *DBM_create(TcpServerController *tsc);
void DBM_add_client_to_DB(TcpClientDBManager *dbm, TcpClient *tcp_client);
void DBM_remove_client_from_DB(TcpClientDBManager *dbm, TcpClient *tcp_client);
void DBM_display(TcpClientDBManager *dbm);
/* void DBM_init_client_db_manager(TcpClientDBManager *dbm); */
void DBM_destroy(TcpClientDBManager *dbm);

#endif
