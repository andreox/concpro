#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main( int argc, char** argv ) {

	typedef int pid_type ;

	pid_type pid ;
	int k ;
	char * s1 = (char*) malloc( 10 * sizeof(char)) ;
	char * s2 = (char*) malloc( 10 * sizeof(char)) ;
	char *nf = (char*) malloc( 20 * sizeof(char)) ;
	while(1) {

		printf("Inserisci nome file (oppure quit)  : ") ;
		scanf("%s",nf) ;
		printf("arg1 : ");
		scanf("%s",s1) ;
		printf("arg2 : ");
		scanf("%s",s2) ;
		bool tr = false ;

		switch( pid = fork() ) {

			case 0 :

				printf("Processo figlio! Eseguo l'exec...\n") ;
				if ( strcmp(nf,"quit") != 0 ) execl( "son" , nf , s1 , s2 ,   (char*) 0 )  ;
				
				else {
					tr = true ;
					 exit(0) ;
				 }

				break ;

			default :
			
				wait(&k) ;
				if( !tr ) printf("Processo padre! Probabilmente mio figlio avra' finito...\n") ;
				else return 0 ;
				break ;

		}

	}

}
