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
 * header length is two. The 'len' for memset should be the same
 * in a similar way.
 *
 * The caller should allocate enough memory for the 'result'.
 */
size_t
UT_get_strlen_as_HDR_string(char *result, char *msg){
    size_t msg_len;

    msg_len = strlen(msg);

    memset(result, '\0', HDR_LEN + 1);

    if (msg_len <= 0 ||
	msg_len >= 100){
	fprintf(stderr,
		"debug : Input message '%s' can't generate valid message header\n",
		msg);
	return 0;
    }

    if (msg_len > 0 && msg_len < 10){
	result[0] = '0';
	result[1] = msg_len + '0'; /* Convert a char to int */
    }else{
	result[0] = (msg_len / 10) + '0';
	result[1] = (msg_len % 10) + '0';
    }

    return msg_len;
}

/* for debug */
void
UT_send_string(int socket_fd, char *msg, int sleep_sec){
    size_t msg_len = strlen(msg);

    if (send(socket_fd, msg, msg_len, 0) < 0){
        perror("send");
        exit(-1);
    }

    if (sleep > 0)
	sleep(sleep_sec);
}

/* Send the header and the main message in separated messages */
void
UT_send_formatted_string(int socket_fd, char *msg, int sleep_sec1, int sleep_sec2){
    char digits[HDR_LEN + 1];
    size_t msg_len;
    ssize_t sent_bytes;

    if ((msg_len = UT_get_strlen_as_HDR_string(digits, msg)) == 0){
	return;
    }

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

/* Send the header and the main message within one message */
void
UT_send_regular_concatenated_string(int socket_fd, char *msg, int sleep_sec){
    /* Allocate memory for null termination as well for safety */
    char msg_with_hdr[MAX_ONE_MESSAGE_SIZE + 1];
    char digits[HDR_LEN + 1];
    size_t msg_len;
    ssize_t sent_bytes;

    memset(msg_with_hdr, '\0', MAX_ONE_MESSAGE_SIZE + 1);

    if ((msg_len = UT_get_strlen_as_HDR_string(digits, msg)) == 0){
	return;
    }

    snprintf(msg_with_hdr, MAX_ONE_MESSAGE_SIZE,
	     "%s%s", digits, msg);

    printf("the concatenated string '%s' will be sent with HDR_LEN + msg_len '%lu'\n",
	   msg_with_hdr, HDR_LEN + msg_len);

    /* Send the header and the main string without the null character */
    if ((sent_bytes = send(socket_fd, msg_with_hdr,
			   HDR_LEN + msg_len, 0)) < 0){
        perror("send");
        exit(-1);
    }

    if (sleep_sec > 0){
        sleep(sleep_sec); /* for debug */
    }
}
