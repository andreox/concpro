//#ifndef __HEADER_H
//#define __HEADER_H

#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define OKLETT 0
#define OKSCRITT 1

typedef struct {

	int temperatura ;
	unsigned int umidita ;
	unsigned short pioggia ;

} meteo ;

typedef struct {

	meteo weather ;
	int num_scritt ;
	int num_lett ;
	Monitor monitor ;

} Monitor_Meteo ;

void init_Monitor_Meteo( Monitor_Meteo* m ) ;
void AggiornaMeteo( Monitor_Meteo* m ) ;
void LeggiMeteo( Monitor_Meteo *m ) ;
void remove_Monitor_Meteo( Monitor_Meteo *m ) ;
