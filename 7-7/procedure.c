#include "header.h"
#include <stdlib.h>
#include <stdio.h>

void StackInit( Stack *s, int dim ) {

	s->dim = dim ;
	s->dati = (int*) malloc( sizeof(int)*dim ) ;
	s->num_occupati = 0 ;

	pthread_mutex_init( &(s->mutex), NULL ) ;
	pthread_cond_init( &(s->okcons), NULL ) ;
	pthread_cond_init( &(s->okprod), NULL ) ;

}

void StackRemove( Stack *s ) {

	free( s->dati ) ;

	pthread_mutex_destroy( &(s->mutex) );
	pthread_cond_destroy( &(s->okcons) ) ;
	pthread_cond_destroy( &(s->okprod ) ) ;

}

void StackPush( Stack *s, Elem e ) {

	pthread_mutex_lock( &(s->mutex ) ) ;

		while ( s->num_occupati == s->dim ) 
			pthread_cond_wait( &(s->okprod), &(s->mutex)) ;

		printf("[PRODUTTORE] Pusho %d in pos %d\n", e, s->num_occupati ) ;
		s->dati[s->num_occupati] = e ;
		s->num_occupati++ ;

		pthread_cond_signal( &(s->okcons) ) ;
	pthread_mutex_unlock( &(s->mutex) ) ;

}

Elem StackPop( Stack *s ) {

	pthread_mutex_lock( &(s->mutex ) ) ;

		while ( s->num_occupati == 0 )
			pthread_cond_wait( &(s->okcons), &(s->mutex)) ;

		s->num_occupati-- ;
		Elem ret_val = s->dati[s->num_occupati] ;
		printf("[CONSUMATORE] Consumo %d in pos %d\n",ret_val, s->num_occupati) ;

		pthread_cond_signal( &(s->okprod)) ;

	pthread_mutex_unlock( &(s->mutex)) ;

	return ret_val ;
}

int StackSize( Stack *s ) { return s->num_occupati ; }
