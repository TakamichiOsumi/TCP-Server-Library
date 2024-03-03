#ifndef __TCP_CLIENT_SERVICE_MANAGER__
#define __TCP_CLIENT_SERVICE_MANAGER__

#include "TcpServerController.h"

typedef struct TcpClientServiceManager {
    TcpServerController *tsc;
} TcpClientServiceManager;

TcpClientServiceManager *DRS_create(TcpServerController *tsc);
void DRS_destroy(TcpClientServiceManager *drs);

#endif
