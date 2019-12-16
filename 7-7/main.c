#include "header.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 5

void * Preleva ( void * s ) ;
void * Produci( void * s ) ;

int main( int argc, char** argv ) {

	pthread_t threads[NUM_THREADS] ;
	pthread_t thread_consumatore ;

	Stack *stack = (Stack*) malloc(sizeof(Stack)) ;

	StackInit( stack, 10 ) ;
	pthread_attr_t attr ;

	pthread_attr_init ( &attr ) ;
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) ;

	int i ;

	for ( i = 0 ; i < NUM_THREADS ; i++ )
		pthread_create( &threads[i], &attr, Produci, (void*) stack ) ;

	pthread_create( &thread_consumatore, &attr, Preleva, (void*) stack ) ;

	for ( i = 0 ; i < NUM_THREADS ; i++ ) 
		pthread_join( threads[i], NULL) ;

	pthread_join( thread_consumatore , NULL ) ;

	pthread_attr_destroy(&attr) ;
	StackRemove( stack ) ;

	free(stack) ;

	pthread_exit(NULL) ;

	return 0 ;

 }

 void * Produci( void * s ) {

 	Stack * stack = (Stack*) s ;
 	srand( time(NULL) ) ;

 	int j ;
 	for ( j = 0 ; j < 4 ; j++ ) {

 		Elem r = rand() % 11 ;
 		StackPush( stack, r) ;
 		sleep(1) ;

 	}

 	pthread_exit(NULL) ;

 }

 void * Preleva( void * s ) {

 	Stack * stack = (Stack*) s ;

 	Elem e1,e2 ;
 	int j ;
 	for ( j = 0 ; j < 10 ; j++ ) {

 		e1 = StackPop( stack ) ;
 		e2 = StackPop( stack ) ;
 		printf("[CONSUMATORE]Stampo la somma %d\n",e1+e2) ;
 		 sleep(1) ;


 	}

 	pthread_exit(NULL) ;
 }