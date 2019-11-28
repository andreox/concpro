#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 5 

typedef struct {

	MonitorVettore *v ;
	MonitorBuffer *b ;

} Monitors ;

void * generatore( void * v ) ;
void * aggiornatore ( void * v ) ;
void * destinatario( void * b ) ;

int main( int argc, char** argv ) {

	pthread_t threads[NUM_THREADS] ;
	pthread_attr_t attr ;
	srand(time(NULL)*getpid()) ;

	Monitors *s ;
	s = (Monitors*) malloc( sizeof(Monitors) ) ;
	s->v = (MonitorVettore*) malloc(sizeof(MonitorVettore) ) ;
	s->b = (MonitorBuffer*) malloc(sizeof(MonitorBuffer) ) ;
	pthread_attr_init( &attr ) ;

	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	pthread_mutex_init( &(s->v->mutex) , NULL ) ;
	pthread_mutex_init( &(s->b->mutex) , NULL ) ;

	pthread_cond_init(  &(s->v->okprod) , NULL ) ;
	pthread_cond_init( &(s->v->okcons), NULL ) ;
	pthread_cond_init( &(s->b->synch) , NULL ) ;

	s->v->testa = 0 ;
	s->v->coda = 0 ;
	s->v->num_occupati = 0 ;
	s->b->num_lettori = 0 ;
	s->b->scrittore = true ;

	int i ;
	pthread_create( &threads[0],  &attr, generatore, (void*) s ) ;
	pthread_create( &threads[1], &attr, aggiornatore, (void*) s) ;
	
	for ( i = 2 ; i < NUM_THREADS ; i++ )
		pthread_create( &threads[i], &attr, destinatario, (void*)s ) ;

	for ( i = 0 ; i < NUM_THREADS ; i++ )
		pthread_join( threads[i], NULL ) ;

	pthread_attr_destroy( &attr ) ;
	pthread_mutex_destroy( &(s->v->mutex) ) ;
	pthread_mutex_destroy( &(s->b->mutex) ) ;
	pthread_cond_destroy( &(s->v->okprod) ) ;
	pthread_cond_destroy( &(s->v->okcons) ) ;
	pthread_cond_destroy( &(s->b->synch) ) ;

	free(s->v) ;
	free(s->b) ;
	free(s) ;

	pthread_exit(NULL) ;

	return 0 ;

}

void * generatore( void * v ) {

	Monitors * vet = (Monitors*) v ;

	elemento e ;
	
	int j ;
	for ( j = 0 ; j < 10 ; j++ ) {

		e.a = rand() % 11 ;
		e.b = rand() % 11 ;
		printf("Genero : %d %d\n",e.a,e.b) ;
		genera( vet->v , e) ;

	}

	pthread_exit(NULL) ;

}

void * aggiornatore( void * v ) {

	Monitors * vet = (Monitors*) v ;

	int j ;
	for ( j = 0 ; j < 10 ; j++ ) {

		elemento k = preleva( vet->v ) ;
		printf("Ho prelevato : %d %d\n",k.a,k.b) ;
		sleep(1) ;
		aggiorna( vet->b, k ) ;

	}

	pthread_exit(NULL) ;

}

void * destinatario( void * v ) {

	Monitors * vet = (Monitors*) v ;
	int j ;
	
	for ( j = 0 ; j < 6 ; j++ ) {

		consulta( vet->b ) ;
		sleep(2) ;

	}

	pthread_exit(NULL) ;

}
