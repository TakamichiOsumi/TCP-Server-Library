#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

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

    close(socket_fd);

    return 0;
}
