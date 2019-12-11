#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {

	MonitorOperandi *mo ;
	MonitorRisultati *mr ;

} Monitor ;

void *Genera( void * m ) ;
void *Prelievo_e_Calcolo( void * m  ) ;
void *Prelievo( void *m ) ;

int main ( int argc, char** argv ) {

	pthread_t thread_generatore ;
	pthread_t thread_calcolatore[2] ;
	pthread_t thread_contatore ;

	pthread_attr_t attr ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	Monitor *m = (Monitor*) malloc(sizeof(Monitor)) ;
	m->mo = (MonitorOperandi*) malloc(sizeof(MonitorOperandi)) ;
	m->mr = (MonitorRisultati*) malloc(sizeof(MonitorRisultati)) ;

	pthread_mutex_init( &(m->mo->mutex), NULL ) ;
	pthread_mutex_init( &(m->mr->mutex), NULL ) ;

	pthread_cond_init( &(m->mr->okprod), NULL ) ;
	pthread_cond_init( &(m->mr->okcons), NULL ) ;
	pthread_cond_init( &(m->mo->okprod), NULL ) ;
	pthread_cond_init( &(m->mo->okcons), NULL ) ;

	m->mo->testa = 0 ;
	m->mo->coda = 0 ;
	m->mo->full = 0 ;

	m->mr->testa = 0 ;
	m->mr->coda = 0 ;
	m->mr->full = 0 ;

	pthread_create( &thread_generatore, &attr, Genera, (void*) m) ;

	int i ;
	for ( i = 0 ; i < 3 ; i++ )
		pthread_create( &thread_calcolatore[i], &attr, Prelievo_e_Calcolo, (void*) m) ;

	pthread_create( &thread_contatore, &attr, Prelievo, (void*) m) ;

	pthread_join( thread_generatore, NULL) ;

	for ( i = 0 ; i < 3 ; i++ )
		pthread_join( thread_calcolatore[i], NULL ) ;

	pthread_join( thread_contatore, NULL ) ;

	pthread_attr_destroy( &attr ) ;
	pthread_mutex_destroy( &(m->mo->mutex) ) ;
	pthread_mutex_destroy( &(m->mr->mutex) ) ;
	pthread_cond_destroy( &(m->mo->okprod) ) ;
	pthread_cond_destroy( &(m->mo->okcons) ) ;
	pthread_cond_destroy( &(m->mr->okprod) ) ;
	pthread_cond_destroy( &(m->mr->okcons) ) ;

	free( m->mo ) ;
	free( m->mr ) ;
	free(m) ;

	pthread_exit(NULL) ;

	return 0 ;

}

void * Genera( void * m ) {

	Monitor* mon = (Monitor*) m ;

	srand( time(NULL) ) ;

	int j ;
	for ( j = 0 ; j < 12 ; j++ ) {
		int r = rand() % 11 ;
		printf("Produco %d\n",r) ;
		inserisci_operando( mon->mo, r) ;

	}

	pthread_exit(NULL) ;
}

void *Prelievo_e_Calcolo( void * m ) {

	Monitor *mon = (Monitor*) m ;

	int j ;
	for ( j = 0 ; j < 4 ; j++ ) {
		int op = preleva_operando( mon->mo ) ;
		printf("Prelevo %d\n",op);
		op *= op ;
		sleep(1);
		inserisci_risultato( mon->mr, op) ;
		printf("Ho inserito %d\n",op) ;
	}

	pthread_exit(NULL) ;

}

void *Prelievo( void * m ) {

	Monitor * mon = (Monitor*) m ;

	int j ;
	int num_pos , num_neg ;
	num_pos = 0 ;
	num_neg = 0 ;
	for ( j = 0 ; j < 12 ; j++ ) {

		int ris = preleva_risultato( mon->mr ) ;

		if ( ris >= 25 ) num_pos++ ;
		else num_neg++ ;

	}

	printf("Numero > 25 : %d\n",num_pos) ;
	printf("Numero < 25 : %d\n",num_neg) ;

	pthread_exit(NULL) ;
}





