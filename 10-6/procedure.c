#include "header.h"

void init_Monitor_Meteo( Monitor_Meteo *m ) {

	m->num_scritt = 0 ;
	m->num_lett = 0 ;
	init_monitor( &(m->monitor), 2 ) ;

}

void AggiornaMeteo( Monitor_Meteo *m ) {

	srand(time(NULL)) ;

	enter_monitor( &(m->monitor) ) ;

		while ( m->num_lett != 0 )
			wait_condition( &(m->monitor), OKSCRITT ) ;

		m->num_scritt++ ;

	leave_monitor( &(m->monitor) ) ;

		m->weather.temperatura = ( rand() % 100 ) - 50 ;
		m->weather.umidita = rand() % 100 ;
		m->weather.pioggia = rand() % 1 ;
		printf("[SCRITTORE] Ho aggiornato in questo modo le variabili : \n") ;
		printf("[SCRITTORE] TEMPERATURA : %d C\n",m->weather.temperatura) ;
		printf("[SCRITTORE] UMIDITA : %d \n",m->weather.umidita) ;
		printf("[SCRITTORE] PIOGGIA : %d\n",m->weather.pioggia) ;

	enter_monitor( &(m->monitor) ) ;

		m->num_scritt-- ;
		signal_condition( &(m->monitor), OKLETT ) ;

	leave_monitor( &(m->monitor) ) ;

}

void LeggiMeteo( Monitor_Meteo *m ) {

	enter_monitor( &(m->monitor) ) ;

		while ( m->num_scritt != 0 )
			wait_condition( &(m->monitor), OKLETT ) ;

		m->num_lett++ ;

	leave_monitor( &(m->monitor) ) ;

	printf("[LETTORE] TEMPERATURA : %d C\n",m->weather.temperatura) ;
	printf("[LETTORE] UMIDITA : %d \n",m->weather.umidita) ;
	printf("[LETTORE] PIOGGIA : %d\n",m->weather.pioggia) ;

	enter_monitor( &(m->monitor)) ;

		m->num_lett-- ;
		if ( m->num_lett == 0 )
			signal_condition( &(m->monitor), OKSCRITT );

	leave_monitor( &(m->monitor) ) ;

}

void remove_Monitor_Meteo( Monitor_Meteo *m ) {

	remove_monitor( &(m->monitor)) ;

}