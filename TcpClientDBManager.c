#include <stdio.h>
#include <stdlib.h>
#include "TcpClientDBManager.h"
#include "TcpClient.h"

static bool
search_tcp_client_by_id(void *p, void *key){
    return false;
}

/*
 * There is no specific memory allocated dynamically for
 * each TcpClient object. Do nothing.
 */
static void
free_tcp_client(void *data){}

TcpClientDBManager *
DBM_create(TcpServerController *tsc){
    TcpClientDBManager *db_manager;

    if ((db_manager = (TcpClientDBManager *)
	 malloc(sizeof(TcpClientDBManager))) == NULL){
	perror("malloc");
	exit(-1);
    }

    db_manager->tsc = tsc;
    db_manager->tcp_client_db = ll_init(search_tcp_client_by_id,
					free_tcp_client);

    return db_manager;
}

void
DBM_display(TcpClientDBManager *dbm){
    node *n;

    printf("------ <Current DB> ------\n");
    ll_begin_iter(dbm->tcp_client_db);
    while((n = ll_get_iter_node(dbm->tcp_client_db)) != NULL){
	TcpClient_print((void *) n->data);
    }
    ll_end_iter(dbm->tcp_client_db);
    printf("--------------------------\n");
}

void
DBM_add_client_to_DB(TcpClientDBManager *dbm,
		     TcpClient *tcp_client){
    ll_insert(dbm->tcp_client_db, (void *) tcp_client);

    printf("debug : the new connection gets cached in TcpClientDBManager by %s\n",
	   __FUNCTION__);
}

/*
void
DBM_init_client_db_manager(TcpClientDBManager *dbm){
}
*/

void
DBM_destroy(TcpClientDBManager *dbm){
}
