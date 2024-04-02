#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TcpClient.h"
#include "CircularByteBuffer/CircularByteBuffer.h"
#include "TcpMessageDemarcation.h"

static bool
MD_is_ready_for_flush(TcpMessageDemarcation *msg_dmrc, size_t required_string_length){
    bool enough_data = msg_dmrc->cbb->used_buffer_size / required_string_length > 0;

    printf("debug : %d / %zu > 0 ? %s\n", msg_dmrc->cbb->used_buffer_size,
	   required_string_length, enough_data ? "yes" : "no");

    if (enough_data){
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

    memset(msg_dmrc->client_message, '\0', circular_buf_size + 1);

    return msg_dmrc;
}

/*
 *
 * The overview of MD_process_message is described below.
 *
 * Step1: The first iteration to handle a message header
 *
 *       CBB_write : recvfrom buffer --- write ---> cbb's main_data
 *                   2 bytes write for the message hdr
 *
 *       CBB_read  : cbb's main_data --- read ---> msg_dmrc->client_message
 *                   2 bytes read for the message hdr
 *                   cbb's main_data is consumed by 'clean_data_source' = true
 *
 *        Store the main message length 'message_length' to msg_dmrc->parsed_msg_length
 *
 * Step2: The second iteration to handle the main message
 *
 *       CBB_write : cbb's main_data <--- write --- recvfrom buffer
 *
 * Step 3: Same as Step2
 *
 *       CBB_read  : cbb's main_data --- read ---> msg_dmrc->client_message
 *
 * Step 4: Same as Step2
 *
 *        Pass the main message 'msg_dmrc->client_message' to the application
 *
 */
void
MD_process_message(TcpMessageDemarcation *msg_dmrc, TcpClient *tcp_client,
		   char *msg_recvd /* passed as recvfrom() argument */,
		   int recv_bytes /* returned by recvfrom() */){
    size_t bytes_read;
    int message_length; /* Obtained by parsing the message header */

    /* Step1 : Parse the header attached to the front of the client message */
    if (msg_dmrc->parsed_header == false){
	assert(msg_dmrc->parsed_msg_length == 0);

	/* Check if the client sends any wrongly formatted data */
	if (recv_bytes != MessageDemarcationHeader){
	    if (recv_bytes <= 1){
		/* The message should be only one byte */
		printf("debug : received bytes '%d' is too short as a message\n",
		       recv_bytes);
		printf("debug : dump received data '%s'\n", msg_recvd);
		return;
	    }else{
		/* The message is more than three bytes */
		if ((msg_recvd[0] < '0' || msg_recvd[0] > '9') ||
		     (msg_recvd[1] < '0' || msg_recvd[1] > '9')){
		    /* But, the header doesn't make sense */
		    printf("debug : received header '%c%c' does not follow the header format\n",
			   msg_recvd[0], msg_recvd[1]);
		    return;
		}
		/* else, the message looks fine at this moment */
		message_length = (msg_recvd[0] - '0') * 10 + (msg_recvd[1] - '0');
		assert(CBB_write(tcp_client->msg_dmrc->cbb,
				 msg_recvd + 2, message_length) > 0);
	    }
	}else{
	    /* The header length is two and fine, quick check for the content */
	    if ((msg_recvd[0] < '0' || msg_recvd[0] > '9') ||
		(msg_recvd[1] < '0' || msg_recvd[1] > '9')){
		printf("debug : received header '%c%c' does not follow the header format\n",
		       msg_recvd[0], msg_recvd[1]);
		return;
	    }

	    /* Read the message header that indicates message length */
	    assert(CBB_write(tcp_client->msg_dmrc->cbb,
			     msg_recvd, MessageDemarcationHeader) > 0);

	    /* Consume and store the header information */
	    bytes_read = CBB_read(msg_dmrc->cbb /* source */,
				  msg_dmrc->client_message /* dest */,
				  MessageDemarcationHeader, true);

	    message_length = atoi(msg_dmrc->client_message);
	}

	msg_dmrc->parsed_header = true;
	msg_dmrc->parsed_msg_length = message_length;

	/*
	 * Reset the client message buffer.
	 *
	 * If the next main message is only one byte message,
	 * then the second header character will be left as garbage and will be passed
	 * to the application.
	 */
	memset(msg_dmrc->client_message, '\0', msg_dmrc->cbb->max_buffer_size + 1);
    }else{
	/*
	 * Step 2: read the main client message, following the previous header sent by client.
	 */
	assert(msg_dmrc->parsed_msg_length != 0);
	/*
	 * Copy characters from recvfrom() internal buffer and increment cbb's internal
	 * counter 'used_buffer_size' by the characters.
	 */
	assert(CBB_write(tcp_client->msg_dmrc->cbb /* dest */, msg_recvd /* source */,
			 msg_dmrc->parsed_msg_length) > 0);
	CBB_dump_snapshot(msg_dmrc->cbb);
    }

    /* Is the recv_bytes bigger than the full message length ? */
    if (!MD_is_ready_for_flush(msg_dmrc, msg_dmrc->parsed_msg_length))
	return;

    /*
     * Step 3 : If the message is long enough, then, copy it as client message.
     */
    bytes_read = CBB_read(msg_dmrc->cbb /* source */, msg_dmrc->client_message /* dest */,
			  msg_dmrc->parsed_msg_length, true);

    msg_dmrc->parsed_header = false;
    msg_dmrc->parsed_msg_length = 0;

    printf("Read %zu bytes by CBB_read in %s\n", bytes_read, __FUNCTION__);

    /*
     * Step 4 : Pass the client message to the application logic.
     */
    if (tcp_client->tsc->received_msg_cb != NULL){
	tcp_client->tsc->received_msg_cb(tcp_client->tsc, tcp_client,
					 msg_dmrc->client_message, bytes_read);
	/*
	 * Clean up the buffer. Otherwise, some junks are left in this buffer,
	 * if the next message length is shorter than this iteration.
	 */
	memset(msg_dmrc->client_message, '\0', msg_dmrc->cbb->max_buffer_size + 1);
    }
}

void
MD_process_fixed_size_message(TcpClient *tcp_client){
}

void
MD_process_variable_size_message(TcpClient *tcp_client){
}
