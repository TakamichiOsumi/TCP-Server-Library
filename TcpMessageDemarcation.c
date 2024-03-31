#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "TcpClient.h"
#include "CircularByteBuffer/CircularByteBuffer.h"
#include "TcpMessageDemarcation.h"

static bool
MD_is_ready_for_flush(TcpMessageDemarcation *msg_dmrc, size_t required_string_length){
    printf("\t %d / %zu > ?\n", msg_dmrc->cbb->used_buffer_size, required_string_length);

    if ((msg_dmrc->cbb->used_buffer_size / required_string_length) > 0){
	return true;
    }else{
	return false;
    }
}

TcpMessageDemarcation *
MD_create_demarcation_instance(TcpMessageDemarcationType dmrc_type, size_t circular_buf_size){
    TcpMessageDemarcation *msg_dmrc;

    msg_dmrc = (TcpMessageDemarcation *) malloc(sizeof(TcpMessageDemarcation));
    if (msg_dmrc == NULL){
	perror("malloc");
	exit(-1);
    }

    msg_dmrc->dmrc_type = dmrc_type;
    msg_dmrc->cbb = CBB_init(circular_buf_size);
    msg_dmrc->parsed_header = false;
    msg_dmrc->client_message = (char *) malloc(sizeof(char) * (circular_buf_size + 1));

    if (msg_dmrc->client_message == NULL){
	perror("malloc");
	exit(-1);
    }

    return msg_dmrc;
}

void
MD_process_message(TcpMessageDemarcation *msg_dmrc, TcpClient *tcp_client,
		   char *msg_recvd, int recv_bytes /* returned by recvfrom() */){
    size_t bytes_read;
    int message_length; /* Obtained by parsing the message header */

    /* Step1 : Parse the header attached to the front of the client message */
    if (msg_dmrc->parsed_header == false){
	assert(msg_dmrc->parsed_msg_length == 0);

	/* Read the message header that indicates message length */
	assert(CBB_write(tcp_client->msg_dmrc->cbb,
		     msg_recvd, MessageDemarcationHeader) > 0);

	/* XXX : replace atoi with strtol */
	message_length = atoi(msg_recvd);

	/* Consume and store the header information */
	bytes_read = CBB_read(msg_dmrc->cbb, msg_dmrc->client_message,
			      2, true);

	msg_dmrc->parsed_header = true;
	msg_dmrc->parsed_msg_length = message_length;
    }else{
	/*
	 * Step 2: read the client message, following the previous header sent by client.
	 */
	assert(msg_dmrc->parsed_msg_length != 0);
	/*
	 * Copy characters from recvfrom() internal buffer and increment cbb's internal
	 * counter 'used_buffer_size' by the characters.
	 */
	assert(CBB_write(tcp_client->msg_dmrc->cbb /* dest */, msg_recvd /* source */,
			 msg_dmrc->parsed_msg_length) > 0);
    }

    /* Is the recv_bytes bigger than the full message length ? */
    if (!MD_is_ready_for_flush(msg_dmrc, msg_dmrc->parsed_msg_length))
	return;

    bytes_read = CBB_read(msg_dmrc->cbb /* source */, msg_dmrc->client_message /* dest */,
			  msg_dmrc->parsed_msg_length, true);

    msg_dmrc->parsed_header = false;
    msg_dmrc->parsed_msg_length = 0;

    printf("Read %zu bytes by CBB_read in %s\n", bytes_read, __FUNCTION__);

    if (tcp_client->tsc->received_msg_cb != NULL)
	tcp_client->tsc->received_msg_cb(tcp_client->tsc, tcp_client,
					 msg_dmrc->client_message, bytes_read);
}

void
MD_process_fixed_size_message(TcpClient *tcp_client){
}

void
MD_process_variable_size_message(TcpClient *tcp_client){
}
