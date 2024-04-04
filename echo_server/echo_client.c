#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../TcpClientUtil.h"

#define BUFFER_SIZE 32

int
main(int argc, char **argv){
    char reply[BUFFER_SIZE];
    int socket_fd;

    memset(reply, '\0', BUFFER_SIZE);

    socket_fd = UT_connect_as_tcp_client("127.0.0.1", 40000);

    UT_send_regular_concatenated_string(socket_fd,
					"Hello World !");

    assert(read(socket_fd, reply, BUFFER_SIZE) > 0);

    printf("Result : '%s'\n", reply);

    UT_close(socket_fd);

    return 0;
}
