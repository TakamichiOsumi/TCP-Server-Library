#ifndef __TCP_MESSAGE_DEMARCATION__
#define __TCP_MESSAGE_DEMARCATION__

/* For definition of TcpClient's */
typedef struct TcpMessageDemarcation TcpMessageDemarcation;

#include "CircularByteBuffer/CircularByteBuffer.h"

typedef enum TcpMessageDemarcationResponse {
    MSG_NG, MSG_OK
} TcpMessageDemarcationResponse;

typedef enum TcpMessageDemarcationType {
    VARIABLE_SIZE
} TcpMessageDemarcationType;

#include "TcpClient.h"

#define MessageDemarcationBufferSize 1024
#define MessageDemarcationHeader 2 /* Define the upper limit of client' message, up to 99.*/

typedef struct TcpMessageDemarcation {

    /* Currently only support variable size communication */
    TcpMessageDemarcationType dmrc_type;

    /*
     * The circular buffer. The internal buffer may contain
     * missing or extra characters.
     */
    CircularByteBuffer *cbb;

    /*
     * If the server already received the header without the data itself,
     * then this flag is set to true.
     */
    bool parsed_header;
    int parsed_msg_length;

    /*
     * This buffer stores the minimum and sufficient string data
     * which the TCP client sends to the server, read and copied
     * from cbb's internal buffer.
     *
     * To prevent any buffer overflow, the size of this buffer
     * is set to cbb's internal buffer + 1, enables full copy of the
     * cbb's buffer. However, in normal cases, the user of client's
     * message should utilize shorter string than that.
     */
    char *client_message;

} TcpMessageDemarcation;

TcpMessageDemarcation *
MD_create_demarcation_instance(TcpMessageDemarcationType dmrc_type,
			       size_t circular_buf_size);
void MD_process_message(TcpMessageDemarcation *msg_dmrc,
			TcpClient *tcp_client, char *msg_recvd,
			int recv_bytes);
void MD_process_fixed_size_message(TcpClient *tcp_client);
void MD_process_variable_size_message(TcpClient *tcp_client);
void MD_destroy(TcpMessageDemarcation *msg_dmrc);

#endif
