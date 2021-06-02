/* -------------------------------------------------
        MÉTODOS DE BÚSQUEDA MULTIARRANQUE
   ------------------------------------------------- */


#ifndef __BMB_H
#define __BMB_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo de Búsqueda Multiarranque Básica en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int busquedaMultiArranque(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness);


#endif