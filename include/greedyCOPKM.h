/* -------------------------------------------------
                COPKM ALGORITHM METHODS
   ------------------------------------------------- */


#ifndef __COPKM_H
#define __COPKM_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo COPKM entero sobre un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos
*/
int greedyCOPKM(PAR &par, int seed, bool mostrarEstado);


#endif