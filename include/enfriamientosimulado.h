/* -------------------------------------------------
        MÉTODOS DE ENFRIAMIENTO SIMULADO
   ------------------------------------------------- */


#ifndef __ES_H
#define __ES_H

#include <iostream>
#include "par.h"


using namespace std;

/*
    Genera el vector de temperaturas
        - Ti: temperatura inicial
        - Tf: temperatura final
        - M:  número de enfriamientos a realizar

    Devuelve el vector de temperaturas
*/
vector<double> generarTemperaturas(double Ti, double Tf, int M);


/*
    Ejecuta el algoritmo de enfriamiento simulado en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int enfriamientoSimulado(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness);


#endif