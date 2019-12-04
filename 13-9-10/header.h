#include "monitor.h"
#include <sys/types.h>

#define DIM 100

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

#define OKPROD 3
#define OKCONS 4

typedef struct {

	unsigned int id_fornitore ;
	unsigned int stato ;

} scaffale ;


struct Magazzino {

	scaffale s[DIM] ;
	int livello_scorte ;
	int free_space ;
	Monitor mon ;

} ;

void InitMagazzino( struct Magazzino *m ) ;
void Fornitura( struct Magazzino *m, pid_t pid ) ;
void Acquisto( struct Magazzino *m ) ;
void RmvMagazzino( struct Magazzino *m ) ;