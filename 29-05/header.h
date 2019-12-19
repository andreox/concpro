#ifndef __HEADER_H
#define __HEADER_H

#include <unistd.h>

#define NORMAL_MESSAGE 1

typedef struct {

	long type ;
	pid_t pid ;
	int a ;
	int b ;

} Msg1 ;

typedef struct {

	long type ;
	int prod ;

} Msg2 ;

#endif