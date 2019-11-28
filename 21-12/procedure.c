#include "header.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void Inizializza( GestioneIO *g ) {

	pthread_attr_init( &(g->attr), NULL ) ;
	pthread_attr_setdetachstate( &(g->attr), PTHREAD_CREATE_JOINABLE ) ;

	pthread_mutex_init( &(g->mutex), NULL ) ;
	pthread_cond_init( &(g->okprod), NULL ) ;
	pthread_cond_init( &(g->okcons), NULL ) ;

	g->testa = 0 ;
	g->coda = 0 ;

	numero_liberi = DIM ;
	numero_occupati = 0 ; 

}

void Produci( GestioneIO *g , Buffer *b ) {

	pthread_mutex_lock( &(g->mutex) ) ;

		while( g->numero_liberi == 0 )
			pthread_cond_wait( &(g->okprod), &(g->mutex)) ;

		g->numero_liberi-- ;

		g->buffer[g->testa].dato = b->dato ;
		g->buffer[g->testa].indirizzo = b->indirizzo ;
		g->testa = (g->testa+1) % DIM ;


	pthread_mutex_unlock( &(g->mutex) ) ;

}

int Consuma( GestioneIO *g, Buffer *b ) {

	pthread_mutex_lock( &( g->mutex ) ) ;

	if( g->numero_liberi == DIM ) {

		pthread_mutex_unlock( &(g->mutex) ) ;

		return 1 ;
	}
		
	g->numero_liberi++ ;

	printf("[C] Consumo elemento in posizione %d : ( %d , %d ) \n", coda, g->buffer[coda].indirizzo, g->buffer[coda].dato ) ;

	g->coda = (g->coda+1) % DIM ;

	pthread_cond_signal( &(g->okprod) ) ;

	pthread_mutex_unlock( &(g->mutex) ) ;

	return 0 ;

}
