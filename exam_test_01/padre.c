#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include "semafori.h"
#include <time.h>
#include "esame.h"

#define MUTEX_DATA 0
#define MUTEX_ISCRITTI 1
#define AGGIORNAMENTO_ISCRIZIONE 2

int main( int argc, char** argv ) {

	key_t key = 29 ;

	int semid = semget( key, 3, IPC_CREAT | 0664 ) ;
	int shmid = shmget( key, sizeof(struct esame) , IPC_CREAT | 0664 ) ;

	semctl( semid, MUTEX_DATA, SETVAL, 1 ) ;
	semctl( semid, MUTEX_ISCRITTI, SETVAL, 1 ) ;
	semctl( semid, AGGIORNAMENTO_ISCRIZIONE, SETVAL, 0) ;
	

	char * attach = NULL ;
	attach = (char*) shmat( shmid, NULL, 0 ) ;

	pid_t PID ;
	int i ;

		PID = fork() ;
		if ( PID == 0 ) {

			execl( "docente" , "docente" , (char*)NULL ) ;
		
		}
	
	for ( i = 0 ; i < 10 ; i++ ) {
		
		PID = fork() ;
	
		if ( PID == 0 ) {
			
			
			execl( "studenti", "a caso",  (char*)NULL ) ;

		}
	}

	for ( i = 0 ; i < 13 ; i++ ) wait(NULL) ;

	semctl( semid, 0 , IPC_RMID ) ;
	shmctl( shmid, IPC_RMID, NULL ) ;
	return 0 ; 
	
}
