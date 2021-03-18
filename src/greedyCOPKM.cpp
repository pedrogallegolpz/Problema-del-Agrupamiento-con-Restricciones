#include <iostream>
#include <vector>
#include <math.h>  
#include <algorithm>
#include "../include/random.h"
#include "../include/greedyCOPKM.h"

using namespace std;

/*
    Ejecuta el algoritmo COPKM entero sobre un problema PAR
*/
void greedyCOPKM(PAR par){
    // Generamos los centroides de forma aleatoria
    Set_random(1);
    vector<vector<double>> centroides;
    vector<double> centroide_aux;

    for(int i=0; i<par.getNumClases(); i++){
        for(int j=0; j<par.getNumAtributos(); j++){
            centroide_aux.push_back(Rand());
        }
        centroides.push_back(centroide_aux);
    }

    // Desordenamos nuestras instancias
    par.shuffleInstances();

    bool hay_cambio=true;
    while(hay_cambio){
        // Asignamos
        
    }

}