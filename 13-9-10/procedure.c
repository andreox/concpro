#include "header.h"
#include <unistd.h>
#include <stdio.h>


void InitMagazzino( struct Magazzino *m ) {

	init_monitor( &(m->mon), 2) ;
	m->livello_scorte = 0 ;
	m->free_space = DIM ;
	int i ;
	for ( i = 0 ; i < DIM ; i++ ) {

		m->s[i].id_fornitore = 0 ;
		m->s[i].stato = LIBERO ;
	}

}

void Fornitura( struct Magazzino *m, pid_t pid) {

	enter_monitor( &(m->mon)) ;

		while( m->free_space == 0 )
			wait_condition( &(m->mon), OKPROD ) ;

		int i = 0 ;
		while ( m->s[i].stato != LIBERO && i < DIM )
			i++ ;

		m->s[i].stato = IN_USO ;
		m->free_space-- ;

	leave_monitor( &(m->mon)) ;

	sleep(2) ;
	m->s[i].id_fornitore = pid ;
	printf("Ho effettuato una fornitura nello scaffale [%d] con PID : %d\n",i, m->s[i].id_fornitore) ;

	enter_monitor( &(m->mon)) ;

		m->s[i].stato = OCCUPATO ;
		m->livello_scorte++ ;
		signal_condition( &(m->mon), OKCONS ) ;

	leave_monitor( &(m->mon) ) ;
}

void Acquisto( struct Magazzino *m ) {

	enter_monitor( &(m->mon) ) ;

		while( m->livello_scorte == 0 )
			wait_condition( &(m->mon), OKCONS ) ;

		int i = 0 ;
		while ( i < DIM && m->s[i].stato != OCCUPATO )
			i++ ;

		m->s[i].stato = IN_USO ;
		m->livello_scorte-- ;

	leave_monitor( &(m->mon) ) ;

	sleep(2) ;
	printf("Un cliente ha effettuato l'acquisto del prodotto nello scaffale [%d]\n",i) ;
	printf("Prodotto acquistato : %d\n",m->s[i].id_fornitore) ;
	m->s[i].id_fornitore = 0 ;


	enter_monitor( &(m->mon) ) ;

		m->s[i].stato = LIBERO ;
		m->free_space++ ;

		signal_condition( &(m->mon), OKPROD ) ;

	leave_monitor( &(m->mon)) ;

}

void RmvMagazzino( struct Magazzino *m ) {

	remove_monitor( &(m->mon) ) ;

}