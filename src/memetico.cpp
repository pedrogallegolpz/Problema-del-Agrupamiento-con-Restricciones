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

    // Limpiamos el problema
    par.clear();
    
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
    while(par.getIterationsFF()<valoraciones_funcion_objetivo){
        par.runEpoch(2,cruce,bls,prob,best);
    }
    
    
    // Finalizamos el algoritmo y nos quedamos con el mejor
    par.finishEpochs();
        
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string t;
    string c;
    

    if(prob==1){
        t="1.0";
    }else{
        t="0.1";
    }
    if(best){
        c="mej";
    }else{
        c="";
    }

    string name = "Memetico(10,"+t+c+")";

    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion();
    cout << "\t" << par.getPoblacion()[0][par.getNumInstancias()] << "(epoca)\n";
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }

    if(mostrarEvolucionFitness){
        // Evolución funcion objetivo
        vector<double> recorrido_fun_objetivo = par.getRecorridoFunObjetivo();

        cout << endl << "EVOLUCIÓN "+name << endl << name+"=[";
        for(int i=0; i<recorrido_fun_objetivo.size()-1;i++){
            cout << recorrido_fun_objetivo[i] << ", ";
        }
        cout << recorrido_fun_objetivo[recorrido_fun_objetivo.size()-1] <<"]\n";
    }

    return elapsed.count();
}
