#include <pthread.h>
#include <unistd.h>

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

typedef struct {	

	int operandi[4] ;
	int totale_operandi ;

} buffer ;

typedef struct {

	buffer elaborazioni[5] ;

	pthread_mutex_t mutex ;

	pthread_cond_t okprod ;
	pthread_cond_t okcons ;

	int numero_liberi ;
	int numero_occupati ;
	int stato[5] ;

} MonitorElaborazioni ;


