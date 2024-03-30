#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "TcpClient.h"
#include "CircularByteBuffer/CircularByteBuffer.h"
#include "TcpMessageDemarcation.h"

static bool
MD_is_ready_for_flush(TcpMessageDemarcation *msg_dmrc){
    if ((msg_dmrc->cbb->used_buffer_size / msg_dmrc->msg_size) > 0){
	return true;
    }else{
	return false;
    }
}

TcpMessageDemarcation *
MD_create_demarcation_instance(TcpMessageDemarcationType dmrc_type,
			       unsigned int cbb_internal_buffer_size){
    TcpMessageDemarcation *msg_dmrc;

    msg_dmrc = (TcpMessageDemarcation *) malloc(sizeof(TcpMessageDemarcation));
    if (msg_dmrc == NULL){
	perror("malloc");
	exit(-1);
    }
    msg_dmrc->dmrc_type = dmrc_type;
    msg_dmrc->cbb = CBB_init(cbb_internal_buffer_size);
    msg_dmrc->buffer = (char *) malloc(sizeof(char) * (cbb_internal_buffer_size + 1));

    if (msg_dmrc->buffer == NULL){
	perror("malloc");
	exit(-1);
    }

    return msg_dmrc;
}

void
MD_process_message(TcpMessageDemarcation *msg_dmrc, TcpClient *tcp_client,
		   char *msg_recvd, size_t msg_size){
    size_t bytes_read;

    /* Copy the message to the cbb */
    assert(CBB_write(tcp_client->msg_dmrc->cbb,
		     msg_recvd, msg_size));
    if (!MD_is_ready_for_flush(msg_dmrc)){
	return;
    }

    while((bytes_read = CBB_read(msg_dmrc->cbb,
				 msg_dmrc->buffer,
				 msg_dmrc->msg_size, true) > 0)){
	printf("Read %zu bytes\n", bytes_read);
    }
    // MD_process_process_fixed_size_message();
    // MD_process_process_variable_size_message();
}

void
MD_process_fixed_size_message(TcpClient *tcp_client){
}

void
MD_process_variable_size_message(TcpClient *tcp_client){
}
