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
*/
void busquedaLocalPAR(PAR par, int seed){
    // Asignamos semilla aleatoria
    Set_random(seed);
    
    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.asignarInstanciasAleatoriamente()){
        cout << "Error al asignar instancias en el algoritmo de búsqueda Local en PAR." << endl;
    }

    
    // Buscamos mejores vecinos hasta llegar al mejor
    //int i=0;
    while(par.buscarPrimerVecinoMejor()){
        //i++;
    }

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    /*
    cout << "Problema BL (seed " << seed<< "):" << endl;
    cout << "\tTime: " << elapsed.count() << " ms" << endl;
    cout << "\tFit function: " << par.fitnessFunction() << endl;
    cout << "\tInfeasibility: " << par.infeasibility() << endl;
    cout << "\tDesviación: " << par.desviacionParticion() << endl;
    */

    cout << "Problema BL (seed " << seed<< "):\t";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion() << endl;

    /*
    cout << "\nFin del algoritmo Búsqueda Local en " << i << " iteraciones" << endl;
    par.mostrarEstado(); 
    */
}