#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "TcpClientUtil.h"

int
UT_connect_as_tcp_client(char *ip_addr, uint16_t port){
    struct sockaddr_in tcp_dest;
    int socket_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&tcp_dest, 0, sizeof(tcp_dest));
    tcp_dest.sin_family = AF_INET;
    tcp_dest.sin_port = htons(port);
    tcp_dest.sin_addr.s_addr = inet_addr(ip_addr);

    if (connect(socket_fd,
                (struct sockaddr *) &tcp_dest,
                sizeof(tcp_dest))  == -1){
        perror("connect");
        exit(-1);
    }

    return socket_fd;
}

void
UT_close(int socket_fd){
    close(socket_fd);
}

/*
 * Note:
 *
 * snprintf inserts a null character at ('size' - 1) index.
 * Hence, if one wants to store double digit safely, then
 * the length of buffer for the output should be three, as the
 * header length two. The 'len' for memset should be the same
 * in a similar way.
 *
 * The caller should allocate enough memory for the 'result'.
 */
size_t
UT_get_strlen_as_HDR_string(char *result, char *msg){
    size_t msg_len;

    msg_len = strlen(msg);

    memset(result, '\0', HDR_LEN + 1);

    printf("debug : strlen(msg) = '%zu'\n", msg_len);

    if (msg_len <= 0 ||
	msg_len >= 100){
	fprintf(stderr,
		"debug : Input message '%s' is out of the range for a message header\n",
		msg);
	return 0;
    }

    if (msg_len > 0 && msg_len < 10){
	result[0] = '0';
	result[1] = msg_len + '0'; /* Convert a char to int */
    }else {
	result[0] = (msg_len / 10) + '0';
	result[1] = (msg_len % 10) + '0';
    }

    return msg_len;
}

void
UT_send_string(int socket_fd, char *msg, int sleep_sec1, int sleep_sec2){
    char *string_with_length[MAX_ONE_MESSAGE_SIZE];
    char digits[HDR_LEN + 1];
    size_t msg_len;
    ssize_t sent_bytes;

    memset(string_with_length, '\0', MAX_ONE_MESSAGE_SIZE);

    msg_len = UT_get_strlen_as_HDR_string(digits, msg);

    /* Send HDR_LEN bytes without the null character */
    if ((sent_bytes = send(socket_fd, digits, HDR_LEN, 0)) < 0){
        perror("send");
        exit(-1);
    }

    if (sleep_sec1 > 0){
        sleep(sleep_sec1); /* for debug */
    }

    if ((sent_bytes = send(socket_fd, msg, strlen(msg), 0) < 0)){
        perror("send");
        exit(-1);
    }

    if (sleep_sec2 > 0){
        sleep(sleep_sec2); /* for debug */
    }
}
