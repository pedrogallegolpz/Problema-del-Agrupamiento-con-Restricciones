#include <iostream>
#include <vector>
#include <math.h>  
#include <chrono>
#include "../include/random.h"
#include "../include/memetico.h"

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
void memetico(PAR par, int tam, int bls, double prob, bool best, int cruce, int seed, bool mostrarEstado){
    // Asignamos semilla aleatoria
    Set_random(seed);

    // Asignamos el tamaño de la población
    par.setTamPoblacion(tam);
    par.setIterationsFF(0);
    
    int valoraciones_funcion_objetivo=100000;    

    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la población
    if(!par.crearPoblacionAleatoria()){
        cout << "Error al crear población aleatoria en el algoritmo genético Local en PAR." << endl;
    }
    
    
    // Buscamos mejores vecinos hasta llegar al mejor
    int borrar=0;
    while(par.getIterationsFF()<valoraciones_funcion_objetivo){
        par.runEpoch(2,cruce,bls,prob,best);
        borrar++;
    }
    cout<< "\nITERACIONES = " << borrar << endl;
    
    
    // Finalizamos el algoritmo y nos quedamos con el mejor
    par.finishEpochs();
        
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string t;
    string c;
    

    if(cruce==1){
        c="U";
    }else{
        c="SF";
    }

    string name = "M.AGG"+c;

    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion() << endl;
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }
}