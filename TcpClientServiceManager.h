#ifndef __TCP_CLIENT_SERVICE_MANAGER__
#define __TCP_CLIENT_SERVICE_MANAGER__

#include "TcpServerController.h"
#include "TcpClient.h"

/* Client Data Request Service, DRS */
typedef struct TcpClientServiceManager {
    TcpServerController *tsc;
} TcpClientServiceManager;

TcpClientServiceManager *DRS_create(TcpServerController *tsc);
void DRS_start_manager_thread(TcpClientServiceManager *drs);
void DRS_listen_comm_fd(TcpClient *tcp_client);
void DRS_destroy(TcpClientServiceManager *drs);

#endif
