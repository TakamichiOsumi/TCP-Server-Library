#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include "TcpServerController.h"
#include "TcpClient.h"

static void app_client_received_msg(TcpServerController *tsc,
				    TcpClient *tcp_client,
				    char *msg, uint16_t msg_size){
    send(tcp_client->comm_fd, msg, msg_size, 0);
}

int
main(int argc, char **argv){
    TcpServerController *tsc;

    tsc = TSC_create("Default TCP Server", "127.0.0.1",
		     40000);

    TSC_set_server_callbacks(tsc, NULL, NULL,
			     app_client_received_msg);

    TSC_start(tsc);

    return 0;
}
