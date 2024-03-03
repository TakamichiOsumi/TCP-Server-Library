
#ifndef __TCP_SERVER_CONTROLLER__
#define __TCP_SERVER_CONTROLLER__

#include <stdint.h>

struct TcpServerController;
typedef struct TcpServerController TcpServerController;

#include "TcpNewConnectionAcceptor.h"
#include "TcpClientServiceManager.h"
#include "TcpClientDBManager.h"

#define NAME_MAX_LEN 64
typedef struct TcpServerController {
    char name[NAME_MAX_LEN];
    uint32_t ip_addr;
    uint16_t port_no;
} TcpServerController;

TcpServerController*
TSC_create(char *name, char *ip_addr, uint16_t port_no);
void TSC_start(TcpServerController* tsc);
void TSC_stop(TcpServerController* tsc);
void TSC_destroy(TcpServerController *tsc);

#endif
