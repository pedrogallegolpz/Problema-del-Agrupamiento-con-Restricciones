#include <iostream>
#include <vector>
#include <math.h>  
#include <algorithm>
#include "../include/random.h"
#include "../include/busqlocal.h"

using namespace std;

/*
    Ejecuta el algoritmo de búsqueda local el primer mejor
    en un problema PAR
*/
void busquedaLocalPAR(PAR par, int seed){
    Set_random(seed);
    
    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.asignarInstanciasAleatoriamente()){
        cout << "Error al asignar instancias en el algoritmo de búsqueda Local en PAR." << endl;
    }

    // Buscamos mejores vecinos hasta llegar al mejor
    int i=0;
    while(par.buscarPrimerVecinoMejor()){
        i++;
    }

    cout << "\nFin del algoritmo Búsqueda Local en " << i << " iteraciones" << endl;
    
    par.mostrarEstado(); 
}