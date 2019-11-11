#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <wait.h>

int main( int argc, char** argv ) {

	//key_t shm_key = 40 ; settato prima così , adesso uso la ftok
	//key_t shm_key = ftok( "./shm_ex" , 's' ) ; dopo aver visto questo vedo ipc_private
	key_t shm_key = IPC_PRIVATE ;
	int ds_shm ; // Shared Memory Descriptor
	//ds_shm = shmget( shm_key , 1024 , 0 ) ; non crea shm 
	//ds_shm = shmget( shm_key , 1024, IPC_CREAT | 0664 ) ;
	ds_shm = shmget( shm_key , 1024 , IPC_CREAT | IPC_EXCL | 0664 ) ;

	if ( ds_shm < 0 ) {

		printf("AAAAAA\n") ;		
		ds_shm = shmget( shm_key , 1024 , 0 ) ;
		
		if ( ds_shm < 0 ) {
			// la risorsa non esiste, in questo caso me lo aspetto perchè ho settato 40 come shm_key e se faccio ipcs non c'è nessuna shm e il flag è 0
			printf("error shm non esiste\n") ;
			exit(1) ;
		}
	}

	
	printf("SHM creata desc: %d\n", ds_shm ) ;
	
	int *shm_ptr = NULL ;
	shm_ptr = (int*) shmat ( ds_shm , NULL, 0 ) ;

	*(shm_ptr) = 1234 ;
	printf("SHM_PTR [%p] = %d\n", shm_ptr , *(shm_ptr) ) ;
	
	int pid ;
	pid = fork() ;
	
	if ( pid == 0 ) {
		printf("sono nel figlio\n") ;
		*(shm_ptr) += 1234 ;
		printf(" SHM_PTR[%p] = %d \n", shm_ptr , *(shm_ptr) ) ;
		exit(0) ; // bisogna metterlo per uscire dal figlio e abilitare il padre che aspetta con la wait

	}

	else {
		
		wait(NULL) ;
		// *(shm_ptr)++ ; ritorna 0 perchè post-incremento ha precedenza su deferenziazione
		*(shm_ptr) += 1234 ;
		printf("shm value : %d\n", *(shm_ptr) ) ;
	}	

	shmctl( ds_shm , IPC_RMID , NULL ) ; 
	return 0 ;

}
