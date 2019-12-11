#include "header.h"
#include <time.h>
#include <stdio.h>

void inserisci_operando( MonitorOperandi *mo, int operando) {

	pthread_mutex_lock( &(mo->mutex)) ;

		while ( mo->full == DIM )
			pthread_cond_wait( &(mo->okprod), &(mo->mutex) ) ;

		mo->operandi[mo->testa] = operando ;
		mo->testa = (mo->testa+1) % DIM ;

		mo->full++ ;
		pthread_cond_signal( &(mo->okcons)) ;

	pthread_mutex_unlock( &(mo->mutex)) ;

}

int preleva_operando( MonitorOperandi *mo ) {

	pthread_mutex_lock( &(mo->mutex) ) ;

		while ( mo->full == 0 ) 
			pthread_cond_wait( &(mo->okcons), &(mo->mutex)) ;

		mo->full-- ;
		int ret_value = mo->operandi[mo->coda] ;
		mo->coda = (mo->coda+1) % DIM ;
		pthread_cond_signal( &(mo->okprod)) ;

	pthread_mutex_unlock( &(mo->mutex) ) ;

	return ret_value ;

}

void inserisci_risultato( MonitorRisultati *mr, int risultato ) {

	pthread_mutex_lock( &(mr->mutex) ) ;

		while ( mr->full == DIM )
			pthread_cond_wait( &(mr->okprod) , &(mr->mutex) ) ;

		mr->risultati[mr->testa] = risultato ;

		mr->testa = ( mr->testa + 1 ) % DIM ;

		mr->full++ ;

		pthread_cond_signal( &(mr->okcons) ) ;

	pthread_mutex_unlock( &(mr->mutex) ) ;

}

int preleva_risultato( MonitorRisultati *mr ) {


	pthread_mutex_lock( &(mr->mutex)) ;

		while ( mr->full == 0 )
			pthread_cond_wait( &(mr->okcons) , &(mr->mutex)) ;

		mr->full-- ;

		int ret_value = mr->risultati[mr->coda] ;
		mr->coda = (mr->coda + 1 ) % DIM ;

		pthread_cond_signal( &(mr->okprod)) ;

	pthread_mutex_unlock( &(mr->mutex) ) ;

	return ret_value ;

}


		

