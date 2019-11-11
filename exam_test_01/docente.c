#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "semafori.h" 
#include <string.h>
#include "esame.h"

#define MUTEX_DATA 0
#define MUTEX_ISCRITTI 1
#define AGGIORNAMENTO_ISCRITTI 2
int main( int argc, char** argv ) {

	char date[3][10] = { "26/05/15" , "27/05/15" , "28/05/15" } ;
	key_t key = 29 ;
	int semid = semget( key, 0 , 0 ) ;
	int shmid = shmget( key, 0, 0 ) ;

	if ( semid == -1 ) printf("Errore nel semaforo\n") ;

	struct esame* attach = NULL ;
	attach = (struct esame*)  shmat( shmid, NULL, 0) ;

	int i ;


	for ( i = 0 ; i < 3 ; i++ ) {
		Wait_Sem( semid, MUTEX_DATA ) ;
	
		printf("Processo docente con PID %d modifica la data\n",getpid()) ;	
		strcpy( attach->data, date[i]) ;
		printf("Adesso la data e' %s\n",attach->data) ;
		Signal_Sem( semid, MUTEX_DATA ) ;

		sleep(3) ;

		Wait_Sem( semid, MUTEX_ISCRITTI ) ;

		printf("Il numero di prenotati e' : %d, ma adesso azzero tutto\n",attach->num_prenotati) ;
		attach->num_prenotati = 0 ;

	 	Signal_Sem( semid, MUTEX_ISCRITTI ) ;

	}

	return 0 ;

}

