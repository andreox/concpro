#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "header.h"

int main( int argc, char** argv ) {

	key_t msg_key_1 = ftok( "." , 'Z' ) ;
	key_t msg_key_2 = ftok( "." , 'W' );

	int status ;

	int msg_id_1 = msgget( msg_key_1, IPC_CREAT | 0664  ) ;

	if ( msg_id_1 < 0 ) {

		printf("Error in creating mess queue 1 : %s\n", strerror(errno)) ;
		exit(0) ;

	}
	
	int msg_id_2 = msgget( msg_key_2, IPC_CREAT | 0664 ) ;

	if ( msg_id_2 < 0 ) {

		printf("Error in creating mess queue 2 : %s\n", strerror(errno)) ;
		exit(0) ;

	}

	//Creo 5 processi client
	pid_t pid ;

	int i ;

	for ( i = 0 ; i < 3 ; i++ ) {

		pid = fork() ;

		if ( pid == -1 ) {

			printf( "Error : %s in creating first child process\n", strerror(errno)) ;
			exit(0) ;

		}

		else if ( pid == 0 ) {

			execl("client","client",NULL) ;
			exit(0) ;

		}
	}

	pid = fork() ;

	if ( pid == -1 ) {

		printf( "Error : %s in creating second child process\n", strerror(errno)) ;
		exit(0) ;

	}

	else if ( pid == 0 ) {

		execl( "server", "server", NULL ) ;
		exit(0) ;

	}

	for ( i = 0 ; i < 3 ; i++ ) {

		wait(&status) ;
		printf("A client process terminated with status %d\n",status) ;

	}

	Msg1 m ;
	m.type = NORMAL_MESSAGE ;
	m.pid = getpid() ;
	m.a = -1 ;
	m.b = -1 ;

	msgsnd( msg_id_1, (void*) &m, sizeof(Msg1) - sizeof(long), 0 ) ;

	wait( &status ) ;
	printf("A server process terminated with status %d\n",status) ;

	int k1 = msgctl( msg_id_1, IPC_RMID, NULL) ;
	if ( k1 ) printf("Message queue 1 removed with success\n") ;
	else printf("Message queue 1 removing op produced an error : %s\n", strerror(errno)) ;

	int k2 = msgctl( msg_id_2, IPC_RMID, NULL ) ;
	if ( k2 ) printf("Message queue 2 removed with success\n") ;
	else  printf( "Message queue 2 removing op produced an error : %s\n", strerror(errno)) ;
	return 0 ;

}