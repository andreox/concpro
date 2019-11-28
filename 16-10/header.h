#include <pthread.h>
#include <stdbool.h>

#define DIM 5

typedef struct {

	int a ;
	int b ;

} elemento ;

typedef struct {

	elemento vettore[DIM] ;

	pthread_mutex_t mutex ;

	pthread_cond_t okprod ;
	pthread_cond_t okcons ;

	int testa ;
	int coda ;
	int num_occupati ;

} MonitorVettore ;

typedef struct {

	elemento buffer ;

	pthread_mutex_t mutex ;

	pthread_cond_t synch ;

	int num_lettori ;
	bool scrittore ;

} MonitorBuffer ;

void genera( MonitorVettore *v, elemento e) ;
elemento preleva( MonitorVettore *v) ;
void aggiorna( MonitorBuffer *b, elemento e ) ;
void consulta( MonitorBuffer *b ) ;

