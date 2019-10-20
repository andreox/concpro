 #include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main( int argc, char** argv ) {

	typedef int pid_type ;

	pid_type pid ;
	int k ;
	char * s1 = (char*) malloc( 10 * sizeof(char)) ;
	char * s2 = (char*) malloc( 10 * sizeof(char)) ;

	switch( pid = fork() ) {

		case 0 :

			printf("Processo figlio! Eseguo l'exec...\n") ;
			printf("Inserisci la prima stringa da passare : ") ;
			scanf("%s",s1 ) ;
			printf("Inserisci la seconda stringa da passare : ") ;
			scanf("%s",s2) ;
			execl( "son" , "son" , s1 , s2 ,   (char*) 0 )  ;
			printf("exec failed\n") ;
			break ;

		default :
			
			wait(&k) ;
			printf("Processo padre! Probabilmente mio figlio avra' finito...\n") ;
			break ;

	}

	return 0 ;

}
