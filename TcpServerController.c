#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "TcpServerController.h"

/* Bundle private variables for TcpServerController */
typedef struct TcpServerSystemComponent {
    TcpNewConnectionAcceptor *acceptor;
    TcpClientDBManager *db_manager;
    TcpClientServiceManager *service_manager;
} TcpServerSystemComponent;

static TcpServerSystemComponent sys_components;

TcpServerController*
TSC_create(char *name, char *ip_addr, uint16_t port_no){
    TcpServerController *tsc;

    if ((tsc = (TcpServerController *) malloc(sizeof(TcpServerController))) == NULL){
	perror("malloc");
	exit(-1);
    }

    strncpy(tsc->name, name, NAME_MAX_LEN);
    tsc->ip_addr = inet_addr(ip_addr);
    tsc->port_no = port_no;

    /* Also, initialize TcpServerSystemComponent variable */
    sys_components.acceptor = CAS_create(tsc);
    sys_components.db_manager = DBM_create(tsc);
    sys_components.service_manager = DRS_create(tsc);

    return tsc;
}

void
TSC_start(TcpServerController* tsc){
}

void
TSC_stop(TcpServerController* tsc){
}

void
TSC_destroy(TcpServerController *tsc){
    if (tsc == NULL)
	return;

    CAS_destroy(sys_components.acceptor);
    DBM_destroy(sys_components.db_manager);
    DRS_destroy(sys_components.service_manager);
}
