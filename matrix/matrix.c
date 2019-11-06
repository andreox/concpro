#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define N 9
#define MESSAGGIO_DISPONIBILE 0
#define MUTEX_SOMMA 1

int main( int argc, char** argv ) {

	int mat[N][N] ;
	int i , j ;
	int Num_Scrittori = N ;
	for ( i = 0 ; i < N ; i++ ) {

		for ( j = 0 ; j < N ; j++ ) mat[i][j] = i;

	}
	
	key_t key = IPC_PRIVATE ;

	int shmid = shmget( key ,sizeof(int)+ sizeof(int)*N*N + sizeof(int)*N + sizeof(int) , IPC_CREAT |  0664 ) ;
	int semid = semget( key , 3 , IPC_CREAT | 0664 ) ;
	int *index = NULL ;

	semctl( semid , MESSAGGIO_DISPONIBILE  , SETVAL , 0 ) ;
	semctl( semid, MUTEX_SOMMA , SETVAL , 0 ) ;

	index = (int*) shmat( shmid , NULL , 0 ) ;
	int *index2 ;
	int *Var_Num_Scrittori ;
	Var_Num_Scrittori = index ;
	*Var_Num_Scrittori = N ;
	index++ ;

	index2 = index ;

	for ( i = 0 ; i < N ; i++ ) {

		for ( j = 0 ; j < N ; j++ ) {

			 *index2 = mat[i][j] ;
			 index2++ ;
		}

	}

	int PID ;
	int * sum = NULL ;
	sum = index2 ;

	for ( i = 0 ; i < N ; i++ ) {

		PID = fork() ;
		if ( PID == 0 ) {

			index += i ;
			for ( j = 0 ; j < N ; j++ ) {

				printf("%d ", *(index) ) ;
				sum[i] += *(index) ;
				index += N ;

			}
			
			(*Var_Num_Scrittori)-- ;
			printf("%d\n",*Var_Num_Scrittori);
			if ( *Var_Num_Scrittori == 0 ) Signal_Sem( semid, MUTEX_SOMMA ) ;

			exit(0) ;

		}

	}

	for ( i = 0 ; i < N ; i++ ) wait(NULL) ;
	
	index++ ;
	*index = 0 ;

	PID = fork() ;
	if ( PID == 0 ) {

		Wait_Sem( semid, MUTEX_SOMMA ) ;
		for ( i = 0 ; i < N ; i++ ) *index += sum[i] ;

		Signal_Sem( semid, MESSAGGIO_DISPONIBILE ) ;
		exit(0) ;

	}

	wait(NULL) ;

	printf("Aspetto che la somma da stampare sia disponibile...\n");
	Wait_Sem( semid, MESSAGGIO_DISPONIBILE ) ;
	sleep(2);
	printf("La somma totale è : %d\n",*index) ;		
			
	semctl( semid , 0 , IPC_RMID ) ;
	shmctl( shmid , IPC_RMID , NULL ) ;
	return 0 ;

}
