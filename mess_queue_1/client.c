#include "header.h"
#include <time.h>

int main ( int argc, char** argv ) {

	srand(time(NULL)) ;

	struct Messaggio m ;
	key_t key = 29 ;
	key_t key1 = 30 ;
	key_t key2 = 31 ;

	m.msg.queue1 = msgget( key, IPC_CREAT | 0664 ) ;
	m.msg.queue2 = msgget( key1, IPC_CREAT | 0664 ) ;
	m.msg.super_queue = msgget( key2, IPC_CREAT | 0664 ) ;

	pid_t pid ;

	int i ;
	for ( i = 0 ; i < 3 ; i++ ) {

		pid = fork() ;

		if ( pid == 0 ) {

			int j ;
			for ( j = 0 ; j < 4 ; j++ ) {

				printf("[CLIENT %d] Sono nel Client\n",j) ;
				int val1 = rand() % 11 ;
				int val2 = rand() % 11 ;
				pid_t s_p = getpid() ;

				struct Messaggio m1,m2 ;
				m1.type = REQUEST_TO_SEND ;
				m1.msg.client_pid = s_p ;
				msgsnd( m.msg.queue1, (void*) &m1, sizeof(struct Messaggio)-sizeof(long),0) ;

				printf("[CLIENT %d] Ho inviato REQUEST_TO_SEND\n",j);
				msgrcv( m.msg.queue2, (void*) &m2, sizeof(struct Messaggio)-sizeof(long), OK_TO_SEND, 0) ;

				printf("[CLIENT %d] Ho ricevuto OK_TO_SEND\n",j) ;
				m.msg.val1 = val1 ;
				m.msg.val2 = val2 ;
				m.type = s_p ;

				msgsnd( m.msg.super_queue, (void*) &m, sizeof(struct Messaggio)-sizeof(long), 0);
				printf("[CLIENT %d] Ho inviato %d,%d con PID : %d\n",j,val1,val2,s_p) ;


			}

			exit(0);
		}
	}

	for ( i = 0 ; i < 3 ; i++ ) wait(0) ;

	msgctl( m.msg.queue1, IPC_RMID, 0);
	msgctl( m.msg.queue2, IPC_RMID, 0) ;
	msgctl( m.msg.super_queue, IPC_RMID, 0) ;

	return 0 ;
}