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
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos
*/
int memetico(PAR &par, int tam, int bls, double prob, bool best, int cruce, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    // Asignamos semilla aleatoria
    Set_random(seed);

    par.clear();
    
    // Asignamos el tamaño de la población
    par.setTamPoblacion(tam);
    par.setIterationsFF(0);
    
    int valoraciones_funcion_objetivo=100000;  

    // Evolución funcion objetivo
    vector<double> fobjetivo_evol;  

    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la población
    if(!par.crearPoblacionAleatoria()){
        cout << "Error al crear población aleatoria en el algoritmo genético Local en PAR." << endl;
    }
        
    // Si queremos mostrar la evolución de la función fitness
    if(mostrarEvolucionFitness){
        vector<vector<double>> p=par.getPoblacion();
        fobjetivo_evol.push_back(p[0][p[0].size()-1]);
    }

    // Buscamos mejores vecinos hasta llegar al mejor
    while(par.getIterationsFF()<valoraciones_funcion_objetivo){
        par.runEpoch(2,cruce,bls,prob,best);
        // Si queremos mostrar la evolución de la función fitness
        if(mostrarEvolucionFitness){
            vector<vector<double>> p=par.getPoblacion();
            fobjetivo_evol.push_back(p[0][p[0].size()-1]);
        }
    }
    
    
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

    if(mostrarEvolucionFitness){
        cout << endl << "EVOLUCIÓN "+name << endl << name+"=[";
        for(int i=0; i<fobjetivo_evol.size()-1;i++){
            cout << fobjetivo_evol[i] << ", ";
        }
        cout << fobjetivo_evol[fobjetivo_evol.size()-1] <<"]\n";
    }

    return elapsed.count();
}
