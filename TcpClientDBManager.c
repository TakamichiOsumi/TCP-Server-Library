#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "TcpClientDBManager.h"
#include "TcpClient.h"

static bool
search_tcp_client_by_id(void *p, void *key){
    TcpClient *tcp_client = (TcpClient *) p;

    if (tcp_client->comm_fd == (uintptr_t) key)
	return true;
    else
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

    pthread_rwlock_init(&db_manager->rwlock, NULL);

    return db_manager;
}

void
DBM_display(TcpClientDBManager *dbm){
    node *n;

    printf("------ <Current DB> ------\n");
    pthread_rwlock_rdlock(&dbm->rwlock);
    ll_begin_iter(dbm->tcp_client_db);
    while((n = ll_get_iter_node(dbm->tcp_client_db)) != NULL){
	TcpClient_print((void *) n->data);
    }
    ll_end_iter(dbm->tcp_client_db);
    pthread_rwlock_unlock(&dbm->rwlock);
    printf("--------------------------\n");
}

void
DBM_add_client_to_DB(TcpClientDBManager *dbm,
		     TcpClient *tcp_client){
    pthread_rwlock_wrlock(&dbm->rwlock);
    ll_insert(dbm->tcp_client_db, (void *) tcp_client);
    pthread_rwlock_unlock(&dbm->rwlock);
    printf("debug : the new connection gets cached in TcpClientDBManager by %s\n",
	   __FUNCTION__);
}

void
DBM_remove_client_from_DB(TcpClientDBManager *dbm, TcpClient *tcp_client){
    pthread_rwlock_wrlock(&dbm->rwlock);
    assert(ll_remove(dbm->tcp_client_db, (void *) tcp_client->comm_fd) != NULL);
    pthread_rwlock_unlock(&dbm->rwlock);
}

/*
void
DBM_delete_client_from_DB(TcpClientDBManager *dbm,
			  void *key){
}
*/

/*
void
DBM_init_client_db_manager(TcpClientDBManager *dbm){
}
*/

void
DBM_destroy(TcpClientDBManager *dbm){
    dbm->tsc = NULL;
    ll_destroy(dbm->tcp_client_db);
    pthread_rwlock_destroy(&dbm->rwlock);
}
