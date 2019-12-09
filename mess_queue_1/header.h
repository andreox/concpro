#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2

typedef struct {

	int queue1 ;
	int queue2 ;
	int super_queue ;
	int val1 ;
	int val2 ;
	pid_t client_pid ;


} msg_t ;


struct Messaggio {

	long type ;
	msg_t msg ;

};

void print( struct Messaggio m ) {

	printf(" q1 : %d\n", m.msg.queue1 ) ;
	printf(" q2 : %d\n",m.msg.queue2 );
	printf(" sq : %d\n",m.msg.super_queue ) ;

}