#include <pthread.h>

typedef int Elem ;

typedef struct {

	Elem * dati ;
	int dim ;

	pthread_mutex_t mutex ;
	pthread_cond_t okprod ;
	pthread_cond_t okcons ;

	int num_occupati ;

} Stack ;

void StackInit( Stack *s, int dim ) ;
void StackRemove( Stack *s ) ;
void StackPush( Stack *s, Elem e) ;
Elem StackPop( Stack *s ) ;
int StackSize( Stack *s ) ;
	