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
