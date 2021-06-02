#include <iostream>
#include <vector>
#include <math.h>  
#include <chrono>
#include "../include/random.h"
#include "../include/busqlocal.h"

using namespace std;

/*
    Ejecuta el algoritmo de búsqueda local el primer mejor
    en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos
*/
int busquedaLocalPAR(PAR &par, int seed, bool mostrarEstado){
    // Asignamos semilla aleatoria
    Set_random(seed);
    par.clear();
    int evaluaciones_max_ff = 100000;

    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.crearSolucionAleatoria()){
        cout << "Error al asignar instancias en el algoritmo de búsqueda Local en PAR." << endl;
    }

    
    // Buscamos mejores vecinos hasta llegar al mejor
    int i=0;
    par.setIterationsFF(0);
    while(par.buscarPrimerVecinoMejor(evaluaciones_max_ff)){
        i++;
    }

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);


    cout << "Problema BL (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion();
    cout << "\tIteracionesBL: " << par.getIterationsFF() << endl;
    


    
    //cout << "\nFin del algoritmo Búsqueda Local en " << par.getIterationsFF() << " iteraciones" << endl;

    if(mostrarEstado){
        par.mostrarEstado(); 
    }
    
    return elapsed.count();
}