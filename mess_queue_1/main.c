#include "header.h"

int main( int argc, char** argv ) {

	key_t key = 29 ;
	key_t key1 = 30 ;
	key_t key2 = 31 ;
	pid_t pid ;
	int queue1 = msgget( key, IPC_CREAT | 0664 ) ;
	int queue2 = msgget( key1, IPC_CREAT | 0664 ) ;
	int super_queue = msgget( key2, IPC_CREAT | 0664 ) ;


	pid = fork() ;
	if ( pid == 0 ) {

		execl("client","client", (char*)NULL) ;

		exit(0) ;
	}

	pid = fork() ;
	if ( pid == 0 ) {

		execl("server","server",(char*)NULL) ;

		exit(0) ;

	}

	int i ;
	for ( i = 0 ; i < 2 ; i++ ) wait(0) ;

	return 0 ;
}