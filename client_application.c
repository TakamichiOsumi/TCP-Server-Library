#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "TcpClientUtil.h"

/* Test to send string data in two separated strings */
static void
CT_send_pair_messages(int socket_fd){
    /* Send one short string */
    UT_send_formatted_string(socket_fd, "Hello Server");
    /* Send one character */
    UT_send_formatted_string(socket_fd, "x");
    /* Send more than 10 characters (only digits) */
    UT_send_formatted_string(socket_fd, "123456789012345");
    UT_send_formatted_string(socket_fd, "Message from client to server");
    /* Could the server handle invalid format messages ? */
    UT_send_string(socket_fd, "0");
    UT_send_string(socket_fd, "0a");
    /* Send one more valid message again, after the invalid strings */
    UT_send_formatted_string(socket_fd, "Message from client to server");
}

static void
CT_send_unified_messages(int socket_fd){
    /* Send one message */
    UT_send_regular_concatenated_string(socket_fd, "One sequence of message");
    /* Send one char */
    UT_send_regular_concatenated_string(socket_fd, "x");
    /* Send another ones */
    UT_send_regular_concatenated_string(socket_fd, "another message");
    UT_send_regular_concatenated_string(socket_fd, "123456789012345");
    UT_send_regular_concatenated_string(socket_fd, "xyz");
}

int
main(int argc, char **argv){
    int socket_fd;

    socket_fd = UT_connect_as_tcp_client("127.0.0.1", 40000);

    CT_send_pair_messages(socket_fd);
    CT_send_unified_messages(socket_fd);

    UT_close(socket_fd);

    return 0;
}
