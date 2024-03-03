#include "TcpServerController.h"

/* Bundle private variables for TcpServerController */
typedef struct TcpServerSystemComponent {
    TcpNewConnectionAcceptor *acceptor;
    TcpClientDBManager *db_manager;
    TcpClientServiceManager *service_manager;
} TcpServerSystemComponent;


TcpServerController*
TSC_create(char *name, uint32_t ip_addr, uint16_t port_no){
    return NULL;
}
void
TSC_start(TcpServerController* tsc){
}

void
TSC_stop(TcpServerController* tsc){
}

void
TSC_destroy(TcpServerController *ts){
}
