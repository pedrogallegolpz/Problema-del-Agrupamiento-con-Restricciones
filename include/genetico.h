/* -------------------------------------------------
            GENETICS ALGORITHMS METHODS
   ------------------------------------------------- */


#ifndef __GENETICO_H
#define __GENETICO_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo Genético entero sobre un problema PAR

        -tam:   tamaño de la población
        -tipo:  1 si es estacionario, 2 generacional
        -cruce: 1 si es uniforme, 2 segmento fijo
        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
*/
void genetico(PAR par, int tam, int tipo, int cruce, int seed, bool mostrarEstado);


#endif