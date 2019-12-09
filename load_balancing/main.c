#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct {

	long type ;
	pid_t pid ;

} Messaggio ;

int main( int argc, char** argv ) {

	key_t key = IPC_PRIVATE ;	
	int q_client_balancer_id = msgget( key, IPC_CREAT | 0664 ) ;
	int q_balancer_server_id[3] ;

	int i ;
	for ( i = 0 ; i < 3 ; i++ ) q_balancer_server_id[i] = msgget(key, IPC_CREAT | 0664 ) ;


	pid_t pid ;

	//Genero i Client
	for ( i = 0 ; i < 2 ; i++ ) {

		pid = fork() ;
		if ( pid == 0 ) {

			int j ;
			for ( j = 0 ; j < 3 ; j++ ) {

				Messaggio m ;
				m.pid = getpid() ;

				printf("[CLIENT %d] Invio un messaggio\n",m.pid) ;
				msgsnd( q_client_balancer_id, (void*) &m, sizeof(Messaggio)-sizeof(long), 0 ) ;
				sleep(1) ;
			}

			exit(0) ;
		}
	}

	//Genero il Balancer
	pid = fork() ;

	if ( pid == 0 ) {

		int k ;
		for ( k = 0 ; k < 6 ; k++ ) {

			Messaggio m1,m2 ;
			msgrcv( q_client_balancer_id, (void*) &m1, sizeof(Messaggio)-sizeof(long), 0, 0) ; //type a 0 perchè prendo il primo messaggio disponibile
			printf("[BALANCER] Ho ricevuto un messaggio, inoltro a Server\n") ;

			m2.pid = m1.pid ;

			msgsnd( q_balancer_server_id[k%3], (void*) &m2, sizeof(Messaggio)-sizeof(long), 0) ;

		}

		exit(0) ;
	}

	for ( i = 0 ; i < 3 ; i++ ) {

		pid = fork() ;

		if ( pid == 0 ) {

			int j ;
			for ( j = 0 ; j < 2 ; j++ ) {

				Messaggio m3 ;
				int ret_val = msgrcv( q_balancer_server_id[i], (void*) &m3, sizeof(Messaggio)-sizeof(long), 0 , 0) ;
				printf("[SERVER %d]Ho ricevuto un messaggio dal Client con PID : %d\n",i,m3.pid) ;

			}

			exit(0) ;

		}

	}

	for ( i = 0 ; i < 6 ; i++ ) wait(0) ;

	for ( i = 0 ; i < 3 ; i++ ) msgctl( q_balancer_server_id[i], IPC_RMID, NULL ) ;
	msgctl( q_client_balancer_id, IPC_RMID, NULL ) ;

	return 0 ;

}