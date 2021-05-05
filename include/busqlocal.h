/* -------------------------------------------------
            LOCAL SEARCH ALGORITHM METHODS
   ------------------------------------------------- */


#ifndef __BL_H
#define __BL_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo de búsqueda local el primer mejor
    en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos
*/
int busquedaLocalPAR(PAR &par, int seed, bool mostrarEstado);


#endif