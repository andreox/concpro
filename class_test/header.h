#include <pthread.h>

#define DIM 5


typedef struct {

	int operandi[DIM] ;
	pthread_mutex_t mutex ;

	pthread_cond_t okprod ;
	pthread_cond_t okcons ;

	int testa ;
	int coda ;

	int full ;

} MonitorOperandi ;

typedef struct {

	int risultati[DIM] ;

	pthread_mutex_t mutex ;
	
	pthread_cond_t okprod ;
	pthread_cond_t okcons ;

	int testa ;
	int coda ;

	int full ;

} MonitorRisultati ;

void inserisci_operando( MonitorOperandi *mo, int operando) ;
int preleva_operando( MonitorOperandi *mo ) ;
void inserisci_risultato( MonitorRisultati *mr, int risultato ) ;
int preleva_risultato( MonitorRisultati *mr ) ;



