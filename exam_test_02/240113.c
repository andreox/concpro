#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define NUM_THREADS 5
#define DISPONIBILE 0
#define AFFITTATO 1

struct DVD {

	int identificativo_film ;
	int identificativo_copia ;
	int stato ;

};

struct Monitor {

	struct DVD dvd[6] ;

	pthread_mutex_t mutex ;

	// non ho bisogno di variabili condition perchè è stato della struct dvd la mia var cond
	pthread_cond_t prod[6] ;
	pthread_cond_t cons[6] ;

};

int affitta( struct Monitor* m, int id_film ) ;
void restituisci( struct Monitor* m, int id_film, int id_copia ) ;

void* stampa( void* m) ;

void* Noleggio_e_Restituzione( void* m ) ;

int main( int argc, char** argv  ) {

	pthread_t threads[NUM_THREADS] ;
	pthread_attr_t attr ;

	struct Monitor * m = malloc( sizeof( struct Monitor )) ;

	int i ;
	m->dvd[0].identificativo_film = 1 ;
	m->dvd[0].identificativo_copia = 1 ;
	m->dvd[0].stato = DISPONIBILE ;

	m->dvd[1].identificativo_film = 1 ;
	m->dvd[1].identificativo_copia = 2 ;
	m->dvd[1].stato = DISPONIBILE ;

	m->dvd[2].identificativo_film = 2 ;
	m->dvd[2].identificativo_copia = 1 ;
	m->dvd[2].stato = DISPONIBILE ;


	m->dvd[3].identificativo_film = 2 ;
	m->dvd[3].identificativo_copia = 2 ;
	m->dvd[3].stato = DISPONIBILE ;

	m->dvd[4].identificativo_film = 3 ;
	m->dvd[4].identificativo_copia = 1 ;
	m->dvd[4].stato = DISPONIBILE ;

	m->dvd[5].identificativo_film = 3 ;
	m->dvd[5].identificativo_copia = 2 ;
	m->dvd[5].stato = DISPONIBILE ;

	pthread_mutex_init( &m->mutex, NULL ) ;
	for ( i = 0 ;i < 6 ; i++ )
		pthread_cond_init( &m->prod[i], NULL ) ;
	
	for ( i = 0 ; i < 6 ; i++ )
		pthread_cond_init( &m->cons[i], NULL ) ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	for ( i = 0 ; i < 5 ; i++ ) {

		if ( i != 4 ) 
			pthread_create( &threads[i], &attr, Noleggio_e_Restituzione, (void*) m ) ;
		else
			pthread_create( &threads[i], &attr, stampa, (void*) m ) ;

	}

	int k ;
	for ( k = 0 ; k < 5 ; k++ )
		pthread_join( threads[k], NULL ) ;

	pthread_mutex_destroy( &m->mutex ) ;
	for ( k = 0 ; k < 6 ; k++ ) { 
		pthread_cond_destroy( &m->cons[k] ) ;
		pthread_cond_destroy( &m->prod[k] ) ;

	}

	free(m) ;
	pthread_exit(NULL) ;

	return 0 ;

}

void * Noleggio_e_Restituzione( void * m ) {

	struct Monitor * mon = ( struct Monitor * ) m ;
	int ret ;

	pthread_mutex_lock( &mon->mutex ) ;

		ret = affitta( mon, 1 ) ;
		
		sleep(1);

	pthread_mutex_unlock( &mon->mutex ) ;

	pthread_mutex_lock( &mon->mutex ) ;

		restituisci( mon, 1, ret ) ;

	pthread_mutex_unlock( &mon->mutex ) ;

	pthread_mutex_lock( &mon->mutex ) ;

		ret = affitta ( mon, 2 ) ;

		sleep(1) ;

	pthread_mutex_unlock( &mon->mutex ) ;

	pthread_mutex_lock( &mon->mutex ) ;

		restituisci( mon, 2, ret ) ;

	pthread_mutex_unlock( &mon->mutex ) ;

	pthread_mutex_lock( &mon->mutex ) ;

		ret = affitta( mon,3 ) ;

		sleep(1) ;

	pthread_mutex_unlock( &mon->mutex ) ;

	pthread_mutex_lock( &mon->mutex ) ;

		restituisci( mon, 3, ret ) ;

	pthread_mutex_unlock( &mon->mutex ) ;

}

int affitta( struct Monitor * m, int id_film ) {

	int index , j ;
	for ( j = 0 ; j < 6 ; j++ ) {

		if ( m->dvd[j].identificativo_film == id_film ) {

			index = j ;
			break ;

		}

	}

	while ( m->dvd[index].stato != DISPONIBILE ) 
		pthread_cond_wait( &m->cons[index], &m->mutex ) ;

	m->dvd[index].stato = AFFITTATO ;
	pthread_cond_signal( &m->prod[index]) ;

	return ( m->dvd[index].identificativo_copia ) ;

}

void restituisci( struct Monitor *m , int id_film, int id_copia ) {

	int j ;
	for ( j = 0 ; j < 6 ; j++ ) 
		if ( m->dvd[j].identificativo_film == id_film && m->dvd[j].identificativo_copia == id_copia ) break ;

	while( m->dvd[j].stato == DISPONIBILE )
		pthread_cond_wait( &m->prod[j], &m->mutex ) ;

	m->dvd[j].stato = DISPONIBILE ;
	pthread_cond_signal( &m->cons[j] ) ;

}

void *stampa( void * m ) {

	struct Monitor * mon = ( struct Monitor *) m ;

  int k ;
	for ( k = 0 ; k < 3 ; k++ ) {

		int j ;

		for ( j = 0 ; j < 6 ; j++ ) {

			printf("Id film : %d\n", mon->dvd[j].identificativo_film ) ;
			printf("Id copia : %d\n", mon->dvd[j].identificativo_copia ) ;
			printf("Stato : %d\n", mon->dvd[j].stato ) ;

		}

		sleep(4) ;

	}

}
