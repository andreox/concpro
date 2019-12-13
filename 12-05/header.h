#include <pthread.h>

#define DIM 3 

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2
typedef struct {

	int identificativo ;
	int quota ;

} Volo ;

typedef struct {

	Volo vettore_voli[DIM] ;
	int vettore_stato[DIM] ;

	pthread_mutex_t mutex ;
	pthread_cond_t okprod ;

	int num_occupati ;

} GestioneVoli ;

void InserisciVolo( GestioneVoli *g, int id ) ;
void RimuoviVolo( GestioneVoli *g, int id) ;
void AggiornaVolo( GestioneVoli *g, int id, int q) ;

