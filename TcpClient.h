#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

/*
 * Enable cross reference between TcpClient and TcpServerController.
 */
struct TcpClient;
typedef struct TcpClient TcpClient;

#include "TcpServerController.h"
#include "TcpMessageDemarcation.h"

typedef struct TcpClient {
    uintptr_t comm_fd;
    uint32_t ip_addr;
    uint16_t port_no;
    TcpServerController *tsc;
    TcpMessageDemarcation *msg_dmrc;
} TcpClient;

TcpClient *
TcpClient_create(int comm_fd, uint32_t ip_addr,
		 uint16_t port_no, TcpServerController *tsc,
		 TcpMessageDemarcation *msg_dmrc);
char *TcpClient_get_abcd_ip(TcpClient *tcp_client);
void TcpClient_print(void *data);

#endif
