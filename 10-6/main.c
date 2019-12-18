#include "header.h"
#include "monitor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>

int main( int argc, char** argv ) {

	key_t shm_key = IPC_PRIVATE ;

	Monitor_Meteo *m = (Monitor_Meteo*) malloc( sizeof(Monitor_Meteo)) ;

	int shmid = shmget( shm_key, sizeof(Monitor_Meteo), IPC_CREAT | 0664 ) ;

	m = (Monitor_Meteo*) shmat( shmid, 0, 0 ) ;

	init_Monitor_Meteo(m) ;

	pid_t pid ;

	pid = fork() ;

	if ( pid == 0 ) {

		int i ;
		for ( i = 0 ; i < 20 ; i++ ) {

			AggiornaMeteo( m ) ;
			sleep(2) ;

		}

		exit(0) ;

	}

	int j ;

	for ( j = 0 ; j < 10 ; j++ ) {

		pid = fork() ;

		if ( pid == 0 ) {

			int k ;
			for ( k = 0 ; k < 10 ; k++ ) {

				LeggiMeteo( m ) ;
				sleep(1) ;

			}

			exit(0) ;
		}
	}

	for ( j = 0 ; j < 11 ; j++ )
		wait(NULL) ;


	shmctl( shmid, IPC_RMID, 0 ) ;
	remove_Monitor_Meteo(m) ;

	return 0 ;

}