#include <stdio.h>
#include "TcpServerController.h"

int
main(int argc, char **argv){
    TcpServerController *tsc;

    tsc = TSC_create("Default TCP Server", "127.0.0.1", 40000);
    TSC_start(tsc);

    printf("initialized the TCP Server Controller\n");

    return 0;
}
