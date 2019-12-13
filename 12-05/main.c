#include "header.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *Esecuzione ( void *g ) ;

struct Parametro {

	GestioneVoli *g ;
	int id ;
} ;

int main( int argc, char** argv ) {

	pthread_t threads[5] ;
	pthread_attr_t attr ;

	struct Parametro *p = (struct Parametro*) malloc(sizeof(struct Parametro)) ;
	p->g = (GestioneVoli*) malloc(sizeof(GestioneVoli)) ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	pthread_mutex_init( &(p->g->mutex ), NULL ) ;
	pthread_cond_init( &(p->g->okprod), NULL ) ;

	int i ;

	p->g->num_occupati = 0 ;
	for ( i = 0 ; i < 3 ; i++ ) {
		p->g->vettore_stato[i] = LIBERO ;
	}

	for ( i = 0 ; i < 5 ; i++ ) {

			pthread_create( &threads[i], &attr, Esecuzione, (void*) p ) ;
			p->id = i ;
	}

	for ( i = 0 ; i < 5 ; i++ )
		pthread_join( threads[i], NULL ) ;

	pthread_cond_destroy( &(p->g->okprod)) ;
	pthread_mutex_destroy( &(p->g->mutex)) ;

	pthread_attr_destroy( &attr ) ;

	free(p->g) ;
	free(p) ;

	pthread_exit(NULL) ;

	return 0 ;

	
}

void *Esecuzione ( void * g ) {

	struct Parametro* p = (struct Parametro*) g ;

	InserisciVolo( p->g, p->id ) ;

	AggiornaVolo( p->g, p->id, 1000 ) ;
	AggiornaVolo( p->g, p->id, 2000 ) ;
	AggiornaVolo( p->g, p->id, 1000 ) ;

	RimuoviVolo( p->g, p->id ) ;

	pthread_exit(NULL) ;

}
