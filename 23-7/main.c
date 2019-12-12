#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>

#define DIM 10 

typedef struct {

	long type ;
	pid_t pid ;
} Msg ;

typedef struct {

	long type ;
	int buffer[DIM] ;
} Msg2 ;

int main( int argc, char** argv ) {

	key_t key = IPC_PRIVATE ;
	int msg_id = msgget( key, IPC_CREAT | 0664 ) ;

	pid_t pid ;

	//Creo 5 Processi Client, ognuno di essi dovrebbe generare 15, per motivi di semplicità ne genererà 4

	int i ;
	for ( i = 0 ; i < 5 ; i++ ) {

		pid = fork() ;

		if ( pid == 0 ) {

			Msg m ;

			int j ;
			for ( j = 0 ; j < 4 ; j++ ) {

				m.type = 1 ;
				m.pid = getpid() ;

				printf("[CLIENT %d] Invia messaggio\n",m.pid) ;
				msgsnd( msg_id, (void*) &m, sizeof(Msg)-sizeof(long), 0) ;
				sleep(1);

			}

			exit(0) ;
		}

	}

	pid = fork() ;

	if ( pid == 0 ) {

		int buffer[DIM] ;
		int r ;

		Msg received ;
		Msg2 sending ;
		sending.type = 2 ;
		int index = 0 ;
		int k = 0 ;
		while ( (r = msgrcv( msg_id, (void*) &received, sizeof(Msg)-sizeof(long), 1, 0)) > 0 && k < 20) {

			sending.buffer[index] = received.pid ;
			index++ ;
			if ( index == DIM ) {

				index = 0 ;
				msgsnd( msg_id, (void*) &sending, sizeof(Msg2)-sizeof(long), 0) ;

			}
			k++ ;
			
		}

		exit(0) ;

	}

	pid = fork() ;

	if ( pid == 0 ) {

		Msg2 printer ;

		int received_msgs = 0 ;
		int r ;
		while ( (r = msgrcv( msg_id, (void*) &printer, sizeof(Msg2)-sizeof(long), 2, 0) ) > 0 && received_msgs < 20 ) {

			received_msgs += ( sizeof(printer.buffer)/sizeof(int) ) ;
			printf("Messaggi ricevuti : %d\n",received_msgs) ;

		}

		exit(0) ;
	}

	for ( i = 0 ; i < 7 ; i++ ) wait(NULL) ;

	msgctl( msg_id, IPC_RMID, 0) ;

	return 0 ;

}