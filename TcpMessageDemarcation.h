#ifndef __TCP_MESSAGE_DEMARCATION__
#define __TCP_MESSAGE_DEMARCATION__

/* For definition of TcpClient's */
typedef struct TcpMessageDemarcation TcpMessageDemarcation;

#include "CircularByteBuffer/CircularByteBuffer.h"

typedef enum TcpMessageDemarcationType {
    FIXED_SIZE,
    VARIABLE_SIZE
} TcpMessageDemarcationType;

#include "TcpClient.h"

typedef struct TcpMessageDemarcation {
    int msg_size; /* for both fixed size and variable size */
    TcpMessageDemarcationType dmrc_type;
    CircularByteBuffer *cbb;
    char *buffer;
} TcpMessageDemarcation;

void MD_init(TcpMessageDemarcation *msg_dmrc);
void MD_process_message(TcpMessageDemarcation *msg_dmrc,
			TcpClient *tcp_client, char *msg_recvd, size_t msg_size);
void MD_process_fixed_size_message(TcpClient *tcp_client);
void MD_process_variable_size_message(TcpClient *tcp_client);
void MD_destroy(TcpMessageDemarcation *msg_dmrc);

#endif