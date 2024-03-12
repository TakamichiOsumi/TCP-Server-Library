#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "TcpServerController.h"
#include "TcpClient.h"

TcpClient *
TcpClient_create(int comm_fd, uint32_t ip_addr,
		 uint16_t port_no, TcpServerController *tsc){
    TcpClient *client;

    if ((client = (TcpClient *) malloc(sizeof(TcpClient))) == NULL){
        perror("malloc");
        exit(-1);
    }

    client->comm_fd = comm_fd;
    client->ip_addr = ip_addr;
    client->port_no = port_no;
    client->tsc = tsc;

    return client;
}

char *
TcpClient_get_abcd_ip(TcpClient *tcp_client){
    struct in_addr ip;

    ip.s_addr = tcp_client->ip_addr;

    return inet_ntoa(ip);
}

/* Callback function for TcpClientDBManager's tcp_client_db list */
void
TcpClient_print(void *data){
    TcpClient *tcp_client = (TcpClient *) data;

    printf("debug : print the connection with client [%s:%d] in %s\n",
	   TcpClient_get_abcd_ip(tcp_client),
	   tcp_client->port_no,
	   __FUNCTION__);
}
