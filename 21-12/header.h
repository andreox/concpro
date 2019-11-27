#include <pthread.h>

#define DIM 10 

typedef struct {

	int indirizzo ;
	int dato ;

} Buffer ;

typedef struct {

	Buffer vettore[DIM] ;

	pthread_mutex_t mutex ;
	pthread_cond_t okprod ;
	pthread_cond_t okcons ;
	pthread_attr_t attr ;

	int testa ;
	int coda ;

	int numero_liberi ;

} GestioneIO ;

void Inizializza( GestioneIO * g ) ;
void Produci( GestioneIO *g, Buffer *b ) ;
int Consuma( GestioneIO *g, Buffer *b ) ;
