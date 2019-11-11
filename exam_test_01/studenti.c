#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "semafori.h"
#include <time.h>
#include "esame.h"

#define MUTEX_DATA 0
#define MUTEX_ISCRITTI 1 
#define AGGIORNAMENTO_ISCRITTI 2

int main( int argc, char** argv ) {

	srand(time(NULL)*getpid()) ;
	key_t key = 29 ;
	int semid = semget( key, 0, 0 ) ;
	int shmid = shmget( key, 0, 0 ) ;

	struct esame * attach = NULL ;
	attach = (struct esame*) shmat( shmid, NULL, 0 ) ;
	
	sleep( rand()%8 ) ;
	Wait_Sem( semid, MUTEX_DATA ) ;

	printf("Stampo la data attuale : ");
	printf("%s",attach->data) ;
	printf("\n") ;

	Signal_Sem( semid, MUTEX_DATA ) ;


	Wait_Sem( semid, MUTEX_ISCRITTI ) ;
	printf("Mi iscrivo alla data. Ho PID : %d\n",getpid()) ;
	attach->num_prenotati++ ;
	Signal_Sem( semid, MUTEX_ISCRITTI ) ;

	return 0 ;

}

