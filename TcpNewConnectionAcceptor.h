#ifndef __TCP_NEW_CONNECTION_ACCEPTOR__
#define __TCP_NEW_CONNECTION_ACCEPTOR__

#include "TcpServerController.h"

typedef struct TcpNewConnectionAcceptor {
    TcpServerController *tsc;
} TcpNewConnectionAcceptor;

TcpNewConnectionAcceptor *CAS_create(TcpServerController *tsc);
void CAS_start_acceptor_thread(TcpNewConnectionAcceptor *cas);
void CAS_destroy(TcpNewConnectionAcceptor *cas);

#endif
