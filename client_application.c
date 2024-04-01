#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "TcpClientUtil.h"

int
main(int argc, char **argv){
    int socket_fd;

    socket_fd = UT_connect_as_tcp_client("127.0.0.1", 40000);

    UT_send_formatted_string(socket_fd, "Hello", 2, 2);
    UT_send_formatted_string(socket_fd, "X", 2, 2);
    UT_send_formatted_string(socket_fd, "FooBarFooBar", 2, 2);
    UT_send_formatted_string(socket_fd, "Server!!", 2, 2);
    UT_send_formatted_string(socket_fd, "from client to server", 2, 2);

    UT_send_string(socket_fd, "0", 2);
    UT_send_string(socket_fd, "0a", 2);

    UT_send_formatted_string(socket_fd, "from client to server", 2, 2);

    UT_close(socket_fd);

    return 0;
}
