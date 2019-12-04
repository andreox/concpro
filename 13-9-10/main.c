#include "header.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main( int argc, char** argv ) {

	key_t shmkey = IPC_PRIVATE ;

	int id_shm = shmget( shmkey, sizeof(struct Magazzino), IPC_CREAT | 0664 ) ;

	struct Magazzino * m = ( struct Magazzino * ) shmat( id_shm, 0, 0) ;

	InitMagazzino(m) ;

	int i ;
	for ( i = 0 ; i < 10 ; i++ ) {

		pid_t pid = fork() ;
		if ( pid == 0 ) {

			int k ;
			for ( k = 0 ; k < 3 ; k++ ) {

				Fornitura(m, getpid()) ;
				sleep(1) ;

			}
			exit(0) ;
		}
	}

	for ( i = 0 ; i < 10 ; i++ ) {

		pid_t pid = fork() ;
		if ( pid == 0 ) {

			int k ;
			for ( k = 0 ; k < 3 ; k++ ) {

				Acquisto(m) ;
				sleep(1) ;
			}

			exit(0) ;
		}
	}

	for ( i = 0 ; i < 20 ; i++ )
		wait(NULL) ;

	RmvMagazzino(m) ;

	shmctl( id_shm, IPC_RMID, 0 ) ;

	printf("Programma padre termina...\n") ;

	return 0 ;


}