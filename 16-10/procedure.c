#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "header.h"

void genera( MonitorVettore *v, elemento e) {

	pthread_mutex_lock( &(v->mutex) ) ;

		while ( v->num_occupati == DIM )
			pthread_cond_wait( &(v->okprod) , &(v->mutex)) ;

		v->num_occupati++ ;
		v->vettore[v->testa] = e ;
		v->testa = (v->testa+1) % DIM ;

		pthread_cond_signal( &(v->okcons) ) ;

	pthread_mutex_unlock( &(v->mutex) ) ;

}

elemento preleva( MonitorVettore *v ) {

	pthread_mutex_lock( &(v->mutex) ) ;

		while( v->num_occupati == 0 )
			pthread_cond_wait( &(v->okcons) , &(v->mutex) ) ;

		v->num_occupati-- ;
		int index = v->coda ;
		v->coda = (v->coda+1) % DIM ;

		pthread_cond_signal( &(v->okprod)) ;

	pthread_mutex_unlock( &(v->mutex) ) ;

	return v->vettore[index] ;

}

void aggiorna( MonitorBuffer *b, elemento e ) {

	pthread_mutex_lock( &(b->mutex) ) ;

		if ( b->num_lettori != 0 )
			pthread_cond_wait( &(b->synch) , &(b->mutex) ) ;

		b->scrittore = true ;

	pthread_mutex_unlock( &(b->mutex) ) ;

	printf("Sto aggiornando il buffer\n");
	b->buffer = e ;

	pthread_mutex_lock( &(b->mutex) ) ;

		b->scrittore = false ;
		pthread_cond_signal( &(b->synch) ) ;

	pthread_mutex_unlock( &(b->mutex)) ;

}

void consulta( MonitorBuffer *b ) {

	pthread_mutex_lock( &(b->mutex) ) ;

		while( b->scrittore )
			pthread_cond_wait( &(b->synch) , &(b->mutex) ) ;

		b->num_lettori++ ;

	pthread_mutex_unlock( &(b->mutex) ) ;

	printf("A : %d \t B : %d\n", b->buffer.a , b->buffer.b ) ;
	printf("A+B : %d\n", ( b->buffer.a ) + ( b->buffer.b ) ) ;

	pthread_mutex_lock( &(b->mutex) ) ;
		
		b->num_lettori-- ;
		pthread_cond_signal( &(b->synch) ) ;

	pthread_mutex_unlock( &(b->mutex)) ;

}
