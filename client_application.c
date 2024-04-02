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
    UT_send_formatted_string(socket_fd, "Hello Server", 2, 2);
    /* Send one character */
    UT_send_formatted_string(socket_fd, "x", 2, 2);
    /* Send more than 10 characters (only digits) */
    UT_send_formatted_string(socket_fd,
			     "1234567890"
			     "12345", 2, 2);
    UT_send_formatted_string(socket_fd,
			     "Message from client to server", 2, 2);
    /* Could the server handle invalid format messages ? */
    UT_send_string(socket_fd, "0", 2);
    UT_send_string(socket_fd, "0a", 2);
    /* Send one more valid message again, after the invalid strings */
    UT_send_formatted_string(socket_fd,
			     "Message from client to server", 2, 2);
}

static void
CT_send_unified_messages(int socket_fd){
    /* Send one message */
    UT_send_regular_concatenated_string(socket_fd, "One sequence of message", 1);
    /* Send another ones */
    UT_send_regular_concatenated_string(socket_fd, "another message", 1);
    UT_send_regular_concatenated_string(socket_fd,
					"1234567890"
					"12345", 1);
    UT_send_regular_concatenated_string(socket_fd,
					"xyz", 1);
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
