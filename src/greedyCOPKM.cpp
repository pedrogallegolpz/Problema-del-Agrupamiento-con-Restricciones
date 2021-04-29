#include <iostream>
#include <vector>
#include <math.h>  
#include <chrono>
#include "../include/random.h"
#include "../include/greedyCOPKM.h"

using namespace std;

/*
    Ejecuta el algoritmo COPKM entero sobre un problema PAR
*/
void greedyCOPKM(PAR par, int seed){
    // Asignamos semilla aleatoria
    Set_random(seed);
    
    // Generamos los centroides de forma aleatoria
    vector<vector<double>> centroides;
    vector<double> centroide_aux;

    Set_random(seed);

    auto begin = chrono::high_resolution_clock::now();
    for(int i=0; i<par.getNumClases(); i++){
        //cout << "Centroide " << i << " tiene de coordenadas:"<< endl;         // Mostrar centroides

        for(int j=0; j<par.getNumAtributos(); j++){
            centroide_aux.push_back(Rand());
            //cout << centroide_aux[j] << " ";                                  // Mostrar centroides
        }
        // cout << endl << endl;                                                // Mostrar centroides
        centroides.push_back(centroide_aux);
        centroide_aux.clear();
    }

    par.setCentroides(centroides);

    // Desordenamos nuestras instancias
    par.shuffleInstances();

    // Ejecutamos el algoritmo
    //int i=0;
    while(par.asignarInstanciasAClustersCercanos()){
        //i++;
    }
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    
    cout << "Problema COPKM (seed " << seed<< "):";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion() << endl;
    
    
    cout << "\nFin del algoritmo Greedy en " << i << " iteraciones" << endl;
    par.mostrarEstado(); 
    
}