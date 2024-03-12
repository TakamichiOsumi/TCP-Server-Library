#ifndef __TCP_CLIENT_SERVICE_MANAGER__
#define __TCP_CLIENT_SERVICE_MANAGER__

#include <pthread.h>
#include <sys/select.h>

#include "TcpServerController.h"
#include "TcpClient.h"
#include "Linked-List/linked_list.h"

/* Client Data Request Service, DRS */
typedef struct TcpClientServiceManager {
    int max_fd;
    fd_set active_fd_set;
    fd_set backup_fd_set;
    pthread_t *service_thread;
    TcpServerController *tsc;
} TcpClientServiceManager;

TcpClientServiceManager *DRS_create(TcpServerController *tsc);
void DRS_start_manager_thread(TcpClientServiceManager *drs);
void DRS_start_listen_comm_fd(TcpClientServiceManager *drs, TcpClient *tcp_client);
void DRS_stop_manager_thread(TcpClientServiceManager *drs);
void DRS_destroy(TcpClientServiceManager *drs);

#endif
