#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

static void
send_string_pair(int socket_fd, char *msg_len, char *msg,
		 int sleep_sec1, int sleep_sec2){

    send(socket_fd, msg_len, 2, 0); /* 2 bytes */

    if (sleep_sec1 > 0){
	sleep(sleep_sec1); /* for debug */
    }

    send(socket_fd, msg, strlen(msg), 0);

    if (sleep_sec2 > 0){
	sleep(sleep_sec2); /* for debug */
    }
}

int
main(int argc, char **argv){
    struct sockaddr_in tcp_dest;
    int socket_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&tcp_dest, 0, sizeof(tcp_dest));
    tcp_dest.sin_family = AF_INET;
    tcp_dest.sin_port = htons(40000);
    tcp_dest.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_fd,
		(struct sockaddr *) &tcp_dest,
		sizeof(tcp_dest))  == -1){
	perror("connect");
	exit(-1);
    }

    send_string_pair(socket_fd, "05", "Hello", 3, 3);
    send_string_pair(socket_fd, "08", "Helloooo", 3, 3);
    send_string_pair(socket_fd, "08", "Server!!", 3, 3);
    send_string_pair(socket_fd, "21", "from client to server", 3, 3);

    close(socket_fd);

    return 0;
}
