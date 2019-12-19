#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "header.h"
#include <pthread.h>

#define DIM 15
pthread_t workers[2] ;

struct Buffer {

	pthread_mutex_t mutex ;
	pthread_cond_t okcons ;
	pthread_cond_t okprod ;

	int num_msg ;
	int index ;
	Msg1 buffer[DIM] ;

} ;

void * Manager ( void * s ) ;
void * Worker( void * s ) ;

int main( int argc, char** argv ) {


	struct Buffer * b = ( struct Buffer *) malloc( sizeof( struct Buffer ) ) ;
	pthread_attr_t attr ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	pthread_mutex_init( &(b->mutex), NULL ) ;
	pthread_cond_init( &(b->okcons), NULL) ;
	pthread_cond_init( &(b->okprod), NULL) ;
	b->index = 0 ;
	b->num_msg = 0 ;

	pthread_t manager ;
	pthread_create( &manager, &attr, Manager, (void*) b ) ;

	int i ;
	for ( i = 0 ; i < 2 ; i++ )
		pthread_create( &workers[i], &attr, Worker, (void*) b) ;

	pthread_join( manager, NULL) ;

	for ( i = 0 ; i < 2 ; i++ )
		pthread_join( workers[i], NULL ) ;

	pthread_attr_destroy( &attr ) ;
	pthread_mutex_destroy( &(b->mutex)) ;
	pthread_cond_destroy( &(b->okcons) ) ;
	pthread_cond_destroy( &(b->okprod) ) ;

	free(b) ;

	pthread_exit(0) ;

	return 0 ;
}

void * Manager( void * s ) {

	struct Buffer * b = ( struct Buffer * ) s ;

	key_t msgkey = ftok( "." , 'Z') ;
	int msg_id = msgget( msgkey, 0) ;
	Msg1 m1 ;
	printf("Sono nel man\n");
	while ( 1 ) {

		sleep(1) ;
		msgrcv( msg_id, (void*) &m1, sizeof(Msg1) - sizeof(long), NORMAL_MESSAGE, IPC_NOWAIT) ;

		if ( m1.a == -1 && m1.b == -1 ) {

			pthread_cancel( workers[0]) ;
			pthread_cancel( workers[1]) ;
			pthread_exit(NULL) ;
		}

		pthread_mutex_lock( &(b->mutex) ) ;

			while ( b->num_msg == DIM ) 
				pthread_cond_wait( &(b->okprod), &(b->mutex) ) ;
			

			b->buffer[b->index] = m1 ;
			b->index++ ;
			b->num_msg++ ;

			pthread_cond_signal( &(b->okcons) ) ;

		pthread_mutex_unlock( &(b->mutex) ) ;

		sleep(1) ;

	}

	free(b) ;
	pthread_exit(NULL) ;
}

void * Worker( void * s ) {

	printf("sono nel worker\n") ;
	struct Buffer * b = ( struct Buffer * ) s ;

	key_t msgkey = ftok( "." , 'W') ;
	int msg_id = msgget( msgkey, 0 ) ;
	Msg1 msg ;

	pthread_mutex_lock( &(b->mutex) ) ;

		while ( b->num_msg == 0 )
			pthread_cond_wait( &(b->okcons), &(b->mutex)) ;

		msg = b->buffer[b->index] ;
		pthread_cond_signal( &(b->okprod)) ;

	pthread_mutex_unlock( &(b->mutex) ) ;

	Msg2 m2 ;
	m2.prod = msg.a * msg.b ;
	m2.type = msg.pid ;

	printf("[WORKER] Sendo %d\n con tipo %ld\n",m2.prod,m2.type) ;
 	msgsnd( msg_id, (void*) &m2, sizeof(Msg2) - sizeof(long), 0) ;
	free(b) ;
	pthread_exit(NULL) ;

}