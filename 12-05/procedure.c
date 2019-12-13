#include "header.h"
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void InserisciVolo( GestioneVoli *g, int id ) {

	pthread_mutex_lock( &(g->mutex) ) ;

		int i = 0 ;

		while ( g->num_occupati == DIM ) 
			pthread_cond_wait( &(g->okprod), &(g->mutex) ) ;

		while ( i < DIM && g->vettore_stato[i] != LIBERO )
			i++ ;

		g->vettore_stato[i] = IN_USO ;
		g->num_occupati++ ;

	pthread_mutex_unlock( &(g->mutex) ) ;

	g->vettore_voli[i].quota = 0 ;
	g->vettore_voli[i].identificativo = id ;
	printf("[INSERIMENTO] Volo %d\n",id) ;
	sleep(1) ;

	pthread_mutex_lock( &(g->mutex) ) ;

		g->vettore_stato[i] = OCCUPATO ;

	pthread_mutex_unlock( &(g->mutex) );

}

void RimuoviVolo( GestioneVoli *g, int id ) {

	pthread_mutex_lock( &(g->mutex) ) ;

		int i = 0 ;
		bool tr = false ;

		while ( i < DIM && !tr ) {

			if ( g->vettore_voli[i].identificativo == id && g->vettore_stato[i] == OCCUPATO  ) tr = true ; 
			else i++ ;

		}

		g->vettore_stato[i] = IN_USO ;

	pthread_mutex_unlock( &(g->mutex) ) ;

	printf("[RIMOZIONE] Volo %d\n",id) ;
	g->vettore_voli[i].quota = 0 ;
	g->vettore_voli[i].identificativo = 0;
	sleep(1) ;

	pthread_mutex_lock( &(g->mutex) ) ;

		g->vettore_stato[i] = LIBERO ;
		g->num_occupati-- ;

		pthread_cond_signal(  &(g->okprod ) ) ;

	pthread_mutex_unlock( &(g->mutex) ) ;

}

void AggiornaVolo( GestioneVoli *g, int id, int q ) {

	srand(time(NULL)) ;

	pthread_mutex_lock( &(g->mutex)) ;

		int i = 0 ;
		bool tr = false ;

		while ( i < DIM && !tr ) {

			if ( g->vettore_voli[i].identificativo == id && g->vettore_stato[i] == OCCUPATO ) tr = true ;
			else i++ ;

		}

		g->vettore_stato[i] = IN_USO ;

	pthread_mutex_unlock( &(g->mutex)) ;

	printf("[AGGIORNAMENTO] Volo %d a quota %d\n",id,q) ;
	g->vettore_voli[i].quota = q ;
	int r = (rand() % 3 ) + 1 ;
	sleep(r) ;

	pthread_mutex_lock( &(g->mutex) ) ;

		g->vettore_stato[i] = OCCUPATO ;

	pthread_mutex_unlock( &(g->mutex) );


}