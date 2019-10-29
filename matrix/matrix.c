#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define N 3

int main( int argc, char** argv ) {

	printf("Inserire elementi matrice : ") ;
	int mat[N][N] ;
	int i , j ;

	for ( i = 0 ; i < N ; i++ ) {

		for ( j = 0 ; j < N ; j++ ) scanf("%d",&mat[i][j]) ;

	}
	
	key_t key = IPC_PRIVATE ;

	int shmid = shmget( key , sizeof(int)*N*N + sizeof(int)*N , IPC_CREAT | IPC_EXCL | 0664 ) ;
	int semid = semget( key , 1 , IPC_CREAT | 0664 ) ;
	
	int *index = NULL ;

	semctl( semid , 0 , SETVAL , 1 ) ;
	index = (int*) shmat( shmid , NULL , 0 ) ;
	int *index2 ;
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

				Wait_Sem( semid , 0 ) ;
				printf("%d ", *(index ) ) ;
				sum[i] += *(index ) ;
				index += N ;
				Signal_Sem( semid , 0 ) ;
			}
			
			printf("\nProcesso %d calcola somma %d \n " , i , sum[i] ) ;
			exit(0) ;

		}

	}

	for ( i = 0 ; i < N ; i++ ) wait(NULL) ;
	
	int tot_sum = 0 ;
	for ( i = 0 ; i < N ; i++ ) tot_sum += sum[i] ;
	printf("La somma totale è : %d\n",tot_sum) ;		
			
	semctl( semid , 0 , IPC_RMID ) ;
	shmctl( shmid , IPC_RMID , NULL ) ;
	return 0 ;

}
