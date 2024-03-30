#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "TcpClient.h"
#include "CircularByteBuffer/CircularByteBuffer.h"
#include "TcpMessageDemarcation.h"

/*
static bool
MD_is_ready_for_flush(void){
    return false;
}*/

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
MD_process_message(TcpClient *tcp_client, char *msg_recvd, size_t msg_size){
}

void
MD_process_fixed_size_message(TcpClient *tcp_client){
}
