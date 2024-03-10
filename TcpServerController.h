
#ifndef __TCP_SERVER_CONTROLLER__
#define __TCP_SERVER_CONTROLLER__

#include <stdint.h>

/*
 * Enable cross references between TcpServerController and
 * TcpNewConnectionAcceptor/TcpClientDBManager/TcpClientServiceManager.
 */
struct TcpServerController;
typedef struct TcpServerController TcpServerController;

#include "TcpClient.h"
#include "TcpNewConnectionAcceptor.h"
#include "TcpClientServiceManager.h"
#include "TcpClientDBManager.h"

typedef void (*client_connected_cb)(TcpServerController *tsc, TcpClient *tcp_client);
typedef void (*client_disconnected_cb)(TcpServerController *tsc, TcpClient *tcp_client);
typedef void (*client_received_msg_cb)(TcpServerController *tsc,
				       TcpClient *tcp_client,
				       char *msg, uint16_t msg_size);

#define NAME_MAX_LEN 64
typedef struct TcpServerController {
    char name[NAME_MAX_LEN];
    uint32_t ip_addr;
    uint16_t port_no;

    client_connected_cb connected_cb;
    client_disconnected_cb disconnected_cb;
    client_received_msg_cb received_msg_cb;

} TcpServerController;

TcpServerController* TSC_create(char *name, char *ip_addr, uint16_t port_no);
void TSC_set_server_callbacks(TcpServerController *tsc,
			      client_connected_cb connected_cb,
			      client_disconnected_cb disconnected_cb,
			      client_received_msg_cb received_msg_cb);
void TSC_start(TcpServerController* tsc);
void TSC_process_new_client(TcpClient *tcp_client);
void TSC_stop(TcpServerController* tsc);
void TSC_destroy(TcpServerController *tsc);

#endif
