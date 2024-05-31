#include <stdio.h>
#include <pthread.h>
#include "../TcpServerController.h"
#include "../TcpClient.h"

static void
app_client_connected(TcpServerController *tsc, TcpClient *tcp_client){

    printf("\n\tapplication : %s has been called\n\n", __FUNCTION__);

    TcpClient_print(tcp_client);
}

static void
app_client_disconnected(TcpServerController *tsc, TcpClient *tcp_client){

    printf("\n\tapplication : %s has been called\n\n", __FUNCTION__);

    TcpClient_print(tcp_client);
}

static void
app_client_received_msg(TcpServerController *tsc, TcpClient *tcp_client,
			char *msg, uint16_t msg_size){

    printf("\n\tapplication : %s has been called\n", __FUNCTION__);
    printf("\tapplication : the message is '%s' & the message size is '%d'\n\n",
	   msg, msg_size);

    TcpClient_print(tcp_client);
}

int
main(int argc, char **argv){
    TcpServerController *tsc;

    tsc = TSC_create("Default TCP Server", "127.0.0.1", 40000);
    TSC_set_server_callbacks(tsc,
			     app_client_connected,
			     app_client_disconnected,
			     app_client_received_msg);
    TSC_start(tsc);

    return 0;
}
