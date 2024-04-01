#ifndef __TCP_CLIENT_UTIL__
#define __TCP_CLIENT_UTIL__

#define HDR_LEN 2
 /*
  * The length of longest string to be sent is the sum of
  * the two-digit header and the longest characters it can represent.
  */
#define MAX_ONE_MESSAGE_SIZE HDR_LEN + 99

#include <stdint.h>

int UT_connect_as_tcp_client(char *ip_addr, uint16_t port);
size_t UT_get_strlen_as_HDR_string(char *result, char *msg);
void UT_send_string(int socket_fd, char *msg,
		    int sleep_sec1, int sleep_sec2);
void UT_close(int socket_fd);

#endif
