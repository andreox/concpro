#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "header.h"
#include <time.h>

int main( int argc, char** argv ) {

	key_t mq_req = ftok( "." , 'W') ;
	key_t mq_send = ftok( "." , 'Z' ) ;

	int msg_id_req = msgget( mq_req, 0) ;
	if ( msg_id_req < 0 )
		printf("Error in creating first message queue : %s\n", strerror(errno)) ;

	int msg_id_send = msgget( mq_send, 0) ;
	if ( msg_id_send < 0 )
		printf("Error in creating second message queue : %s\n", strerror(errno)) ;

	srand(time(NULL)) ;

	int i ;
	pid_t pid = getpid() ;
	Msg1 m1 ;
	Msg2 m2 ;
	int status ;

	for ( i = 0 ; i < 5 ; i++ ) {

		m1.type = NORMAL_MESSAGE ;
		m1.a = rand() % 11 ;
		m1.b = rand() % 11 ;
		m1.pid = pid ;

		status = msgsnd( msg_id_send, (void*) &m1, sizeof(Msg1) - sizeof(long), 0) ;
		if ( status != -1 ) printf("[CLIENT %d] Ho inviato A : %d e B : %d\n", m1.pid, m1.a, m1.b) ;
		else {
			printf("[CLIENT %d] Error in sending message : %s\n", pid, strerror(errno) ) ;
			exit(0) ;

		}

		status = msgrcv( msg_id_req, (void*) &m2, sizeof(Msg2) - sizeof(long), pid, 0) ;

		if ( status != -1 ) printf("[CLIENT %d] Ho ottenuto il prodotto : %d\n", pid, m2.prod) ;
		else {
			printf("[CLIENT %d] Error in receiving message : %s\n", pid, strerror(errno) ) ;
			exit(0) ;
		}

	}

	int k1 = msgctl( msg_id_req, IPC_RMID, NULL) ;
	if ( k1 ) printf("Message queue 1 removed with success\n") ;
	else printf("Message queue 1 removing op produced an error : %s\n", strerror(errno)) ;

	int k2 = msgctl( msg_id_send, IPC_RMID, NULL ) ;
	if ( k2 ) printf("Message queue 2 removed with success\n") ;
	else  printf( "Message queue 2 removing op produced an error : %s\n", strerror(errno)) ;
	return 0 ;

}