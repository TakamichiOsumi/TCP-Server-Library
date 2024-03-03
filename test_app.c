#include <stdio.h>
#include "TcpServerController.h"

int
main(int argc, char **argv){
    TcpServerController *tc;

    tc = TSC_create(NULL, 0, 0);

    printf("initialized the TCP Server Controller\n");

    return 0;
}
