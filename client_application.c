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
    UT_send_formatted_string(socket_fd,
			     "ABCDEFGHIJ"
			     "KLMNO");
    /* Send one character */
    UT_send_formatted_string(socket_fd, "A");
    /* Send more than 10 characters (only digits) */
    UT_send_formatted_string(socket_fd, "123456789012345");
    UT_send_formatted_string(socket_fd,
			     "abcdefghij"
			     "klmno");
    /* Could the server handle invalid format messages ? */
    UT_send_string(socket_fd, "0");
    UT_send_string(socket_fd, "0a");
    /* Send one more valid message again, after the invalid strings */
    UT_send_formatted_string(socket_fd, "000001234567890");
}

static void
CT_send_unified_messages(int socket_fd){
    /* Send one message */
    UT_send_regular_concatenated_string(socket_fd,
					"ABCDEFGHIJ"
					"KLMNO");
    /* Send one char */
    UT_send_regular_concatenated_string(socket_fd, "A");
    /* Send other ones */
    UT_send_regular_concatenated_string(socket_fd, "123456789012345");
    UT_send_regular_concatenated_string(socket_fd, "ABC");
}

/* Conduct stress test */
static void
CT_send_many_messages(int socket_fd){
#define MSG_LEN 17

    char msg[MSG_LEN]; /* "Hello World XXX", where XXX is a number */
    int i;

    for (i = 0; i < 1000; i++){
	snprintf(msg, MSG_LEN, "%s %04d", "Hello World", i);
	UT_send_regular_concatenated_string(socket_fd, msg);
    }
}

static void
CT_send_separated_messages(int socket_fd){
    UT_send_plain_string(socket_fd, "10"); /* the header */
    UT_send_plain_string(socket_fd, "123");
    UT_send_plain_string(socket_fd, "456");
    UT_send_plain_string(socket_fd, "789");
    UT_send_plain_string(socket_fd, "0");

    UT_send_plain_string(socket_fd, "05"); /* the header */
    UT_send_plain_string(socket_fd, "1");
    UT_send_plain_string(socket_fd, "2");
    UT_send_plain_string(socket_fd, "3");
    UT_send_plain_string(socket_fd, "4");
    UT_send_plain_string(socket_fd, "5");

    /*
     * Note:
     *
     * Create some tests to handle splitted header case, like one header's
     * first character is concatenated with the previous message
     * and the second character is send separately, or
     * one header is completely included in a previous message, etc.
     *
     * Some ideas for further tests can be described like below.
     *
     * (case 1) "05" "ABCDE03" "a" "b" "c"
     * (case 2) "05" "ABCDE1" "01" "234567890"
     * (case 3) "0" "5ABC" "DE"
     *
     * On the other hand, addressing these kinds of input require some updates
     * of library code and additionally the current series of tests prove
     * that following the message format of this library, checking the server
     * side responses ('C' or 'F'), ensures the server application's acceptance
     * of the client's messages.
     *
     * Therefore, those tests will be skipped at this moment.
     * If some clear needs appear, then those would be addressed.
     */
}

int
main(int argc, char **argv){
    int socket_fd;

    socket_fd = UT_connect_as_tcp_client("127.0.0.1", 40000);

    CT_send_pair_messages(socket_fd);
    CT_send_unified_messages(socket_fd);
    CT_send_many_messages(socket_fd);
    CT_send_separated_messages(socket_fd);

    UT_close(socket_fd);

    return 0;
}
