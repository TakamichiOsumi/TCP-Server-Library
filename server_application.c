#include <stdio.h>
#include <pthread.h>
#include "TcpServerController.h"

int
main(int argc, char **argv){
    TcpServerController *tsc;

    tsc = TSC_create("Default TCP Server", "127.0.0.1", 40000);

    TSC_start(tsc);

    printf("debug : initialized the TCP Server Controller\n");

    pthread_exit(0);

    return 0;
}
