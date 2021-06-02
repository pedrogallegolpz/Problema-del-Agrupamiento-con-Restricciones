/* -------------------------------------------------
        MÉTODOS DE BÚSQUEDA LOCAL REITERADA
   ------------------------------------------------- */


#ifndef __ILS_H
#define __ILS_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo de Búsqueda Local Reiterada en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int busquedaLocalReiterada(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness);


#endif