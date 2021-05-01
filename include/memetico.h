/* -------------------------------------------------
            GENETICS ALGORITHMS METHODS
   ------------------------------------------------- */


#ifndef __MEMETICO_H
#define __MEMETICO_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Ejecuta el algoritmo Memético con AGG entero sobre un problema PAR

        -tam:   tamaño de la población
        -bls:   número que indica cada cuántas épocas se ejecuta la búsqueda local suave
        -prob:  probabilidad de que se modifique un cromosoma
        -best:  booleano: se cogen solo los prob*tam mejores si true (prob!=1)
        -cruce: 1 si es uniforme, 2 segmento fijo
        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
*/
void memetico(PAR par, int tam, int bls, double prob, bool best, int cruce, int seed, bool mostrarEstado);


#endif