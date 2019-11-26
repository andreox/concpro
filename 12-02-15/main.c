#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <time.h>


#define NUM_THREADS 6
void * Richiedente( void * m ) ;
void * Elaboratore( void * m ) ;


int main( int argc, char** argv ) {

	pthread_t threads[NUM_THREADS] ;
	pthread_attr_t attr ;

	pthread_attr_init( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;
	srand(time(NULL)*getpid()) ;

	MonitorElaborazioni * monitor = ( MonitorElaborazioni* ) malloc( sizeof(MonitorElaborazioni) ) ;

	pthread_mutex_init( &(monitor->mutex) , NULL ) ;
	pthread_cond_init( &(monitor->okprod) , NULL ) ;
	pthread_cond_init( &(monitor->okcons), NULL ) ;

	int i ;

	for ( i = 0 ; i < 5 ; i++ ) monitor->stato[i] = LIBERO ;
	monitor->numero_liberi = 4 ;
	monitor->numero_occupati = 0 ;

	for ( i = 0 ; i < 4 ; i++ ) pthread_create( &threads[i], &attr, Richiedente, (void*) monitor ) ;
	for ( ; i < 6 ; i++ ) pthread_create( &threads[i], &attr, Elaboratore, (void*) monitor ) ;

	pthread_attr_destroy( &attr ) ;

	for ( i = 0 ; i < 6 ; i++ ) pthread_join( threads[i], NULL ) ;

	pthread_cond_destroy( &monitor->okprod ) ;
	pthread_cond_destroy( &monitor->okcons ) ;
	pthread_mutex_destroy( &monitor->mutex ) ;
	free( monitor ) ;
	pthread_exit(NULL ) ;


	return 0 ;

}

void * Richiedente ( void * m ) {

	MonitorElaborazioni * mtr = (MonitorElaborazioni*) m ;

	int j ;
	for ( j = 0 ; j < 3 ; j++ ) {
		pthread_mutex_lock( &(mtr->mutex) ) ;

			int	k = 0 ;
			while ( mtr->numero_liberi == 0 ) 
				pthread_cond_wait( &(mtr->okprod) , &(mtr->mutex) ) ;

			while ( k < 5 && mtr->stato[k] != LIBERO ) 
				k++ ;


			 mtr->stato[k] = IN_USO ;
			
			 mtr->numero_liberi-- ;
			
		pthread_mutex_unlock( &(mtr->mutex) ) ;

		mtr->elaborazioni[k].totale_operandi = rand()%2 + 2 ;
		printf("Elaboratore %d produce %d operandi\n",k,mtr->elaborazioni[k].totale_operandi ) ;
		int i ;

		for ( i = 0 ; i < mtr->elaborazioni[k].totale_operandi ; i++ ) {
			
			mtr->elaborazioni[k].operandi[i] = rand() % 11 ;
			printf("Elaboratore %d produce operando %d n. %d\n",k, mtr->elaborazioni[k].operandi[i] , i ) ;

		}

		pthread_mutex_lock( &(mtr->mutex) ) ;

			mtr->numero_occupati++ ;
			mtr->stato[k] = OCCUPATO ;
			pthread_cond_signal( &(mtr->okcons) ) ;

		pthread_mutex_unlock( &(mtr->mutex) ) ;

	}
	pthread_exit(NULL) ;

}

void * Elaboratore ( void * m ) {

	MonitorElaborazioni * mtr = (MonitorElaborazioni*) m ;

	int j ;
	for ( j = 0 ; j < 6 ; j++ ) {
		pthread_mutex_lock( &(mtr->mutex) ) ;

			int k = 0 ;

			while ( mtr->numero_occupati == 0 ) 
				pthread_cond_wait( &(mtr->okcons) , &(mtr->mutex) ) ;

			while ( k < 5 && mtr->stato[k] != OCCUPATO )
				k++ ;

			mtr->stato[k] = IN_USO ;
			mtr->numero_occupati-- ;

		pthread_mutex_unlock( &(mtr->mutex) ) ;

		int i ;
		int sum = 0 ;
		sleep(2) ;
		for ( i = 0 ; i < mtr->elaborazioni[k].totale_operandi  ; i++ ) {
			
			sum += mtr->elaborazioni[k].operandi[i]  ;
			printf(" operando %d dell'elaborazione %d : %d\n", i , k , mtr->elaborazioni[k].operandi[i]) ;

		}

		printf(" somma degli operandi dell'elaborazione %d : %d\n",k,sum) ;

		pthread_mutex_lock( &(mtr->mutex) ) ;

			mtr->numero_liberi++ ;
			mtr->stato[k] = LIBERO ;
			pthread_cond_signal( &(mtr->okprod) ) ;

		pthread_mutex_unlock( &(mtr->mutex) ) ;

	}
	pthread_exit(NULL) ;

}
		
