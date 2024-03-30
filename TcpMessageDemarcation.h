#ifndef __TCP_MESSAGE_DEMARCATION__
#define __TCP_MESSAGE_DEMARCATION__

/* For definition of TcpClient's */
typedef struct TcpMessageDemarcation TcpMessageDemarcation;

typedef enum TcpMessageDemarcationType {
    FIXED_SIZE,
    VARIABLE_SIZE
} TcpMessageDemarcationType;

#include "TcpClient.h"

typedef struct TcpMessageDemarcation {
    TcpMessageDemarcationType dmrc_type;
    CircularByteBuffer *cbb;
    char *buffer;
} TcpMessageDemarcation;

void MD_init(void);
void MD_process_message(TcpClient *tcp_client,
			char *msg_recvd, size_t msg_size);
void MD_process_fixed_size_message(TcpClient *tcp_client);
void MD_destroy(TcpMessageDemarcation *msg_dmrc);

#endif
