#include "header.h"

void * Server( void * m ) {

	struct Messaggio *mess = (struct Messaggio*) m ;

	struct Messaggio m1,m2 ;

	m2.type = OK_TO_SEND ;

	int j ;
	for ( j = 0 ; j < 6 ; j++ ) {

		printf("[SERVER %d] Aspetto REQUEST_TO_SEND...\n",j) ;
		//print( *mess ) ;
		msgrcv( mess->msg.queue1, (void*) &m1, sizeof(struct Messaggio)-sizeof(long), REQUEST_TO_SEND, 0 ) ;
		long client = (long) m1.msg.client_pid ;
		msgsnd( mess->msg.queue2, (void*) &m2, sizeof(struct Messaggio)-sizeof(long), 0) ;
		msgrcv( mess->msg.super_queue, (void*) mess, sizeof(struct Messaggio)-sizeof(long), client, 0) ;

		printf("[SERVER %d] Messaggio generato dal Client : %ld\n Somma dei valori generati : %d \n",j,client, mess->msg.val1 + mess->msg.val2 ) ;

	}

	pthread_exit(NULL) ;

}

int main( int argc, char** argv ) {

	pthread_t threads[2] ;
	key_t key = 29 ;
	key_t key1 = 30 ;
	key_t key2 = 31 ;


	struct Messaggio *m = (struct Messaggio*) malloc( sizeof(struct Messaggio) ) ;

	m->msg.queue1 = msgget( key, IPC_CREAT | 0664 ) ;
	m->msg.queue2 = msgget( key1, IPC_CREAT | 0664 ) ;
	m->msg.super_queue = msgget( key2, IPC_CREAT | 0664 ) ;

	pthread_attr_t attr ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	int i ;
	for ( i = 0 ; i < 2 ; i++ ) 
		pthread_create( &threads[i] , &attr, Server, (void*) m ) ;

	for ( i = 0 ; i < 2 ; i++ ) 
		pthread_join( threads[i], NULL );

	for ( i = 0 ; i < 2 ; i++ )
		wait(NULL) ;


	msgctl( m->msg.queue1, IPC_RMID, 0);
	msgctl( m->msg.queue2, IPC_RMID, 0) ;
	msgctl( m->msg.super_queue, IPC_RMID, 0) ;

	pthread_attr_destroy( &attr ) ;
	free(m) ;
	pthread_exit(NULL);

	return 0 ;

}