#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "TcpServerController.h"
#include "TcpNewConnectionAcceptor.h"
#include "TcpClientServiceManager.h"

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
TSC_set_server_callbacks(TcpServerController *tsc,
			 client_connected_cb connected_cb,
			 client_disconnected_cb disconnected_cb,
			 client_received_msg_cb received_msg_cb){
    tsc->connected_cb = connected_cb;
    tsc->disconnected_cb = disconnected_cb;
    tsc->received_msg_cb = received_msg_cb;
}

void
TSC_start(TcpServerController* tsc){
    /* Start the CRS thread, the DRS thread and initialize the DBMS */
    CAS_start_acceptor_thread(sys_components.acceptor);
    DRS_start_manager_thread(sys_components.service_manager);
    // DBM_init_client_db_manager(sys_components.db_manager);
}

void
TSC_process_new_client(TcpClient *tcp_client){
    DBM_add_client_to_DB(sys_components.db_manager,
			 tcp_client);
    //DRS_listen_comm_fd()
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
