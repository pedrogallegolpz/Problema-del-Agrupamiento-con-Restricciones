#include <iostream>
#include <vector>
#include <math.h>  
#include <cmath>
#include <chrono>
#include "../include/random.h"
#include "../include/busqlocreit.h"

using namespace std;

/*
    Ejecuta el algoritmo de Búsqueda Local Reiterada en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int busquedaLocalReiterada(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    Set_random(seed);

    // Limpiamos el problema
    par.clear();

    int evaluaciones_max_ff = 10000;  

    // Evolución funcion objetivo
    vector<double> inicios_fit = par.getPeoresFitnessTrayectoria();
    vector<double> finales_fit = par.getMejoresFitnessTrayectoria();

    // Inicializamos constantes
    int size = 0.1*par.getNumInstancias();


    auto begin = chrono::high_resolution_clock::now();
    par.setIterationsFF(0);

    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.crearSolucionAleatoria()){
        cout << "Error al asignar instancias aleatoriamente en el algoritmo de enfriamiento simulado." << endl;
    }
    inicios_fit.push_back(par.getFuncionObjetivo());

    // Iniciamos búsqueda
    while(par.buscarPrimerVecinoMejor(evaluaciones_max_ff)){
    }
    finales_fit.push_back(par.getFuncionObjetivo());


    // Comenzamos el bucle con soluciones obtenidas anteriormente
    for(int i=0; i<9; i++){
        par.setIterationsFF(0);

        // Mutamos
        par.mutacionILS(size);
        inicios_fit.push_back(par.getFuncionObjetivo());

        // Iniciamos búsqueda
        while(par.buscarPrimerVecinoMejor(evaluaciones_max_ff)){
        }
        finales_fit.push_back(par.getFuncionObjetivo());

    }        

    par.simularMejorSolucion();

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string name = "ILS";
    
    cout << "Problema "+name+" (seed " << seed<< "):   ";
    cout << "\t" << elapsed.count() << "ms";
    cout << "\t" << par.fitnessFunction();
    cout << "\t" << par.infeasibility();
    cout << "\t" << par.desviacionParticion();
    cout << endl;
    
    if(mostrarEstado){
        par.mostrarEstado(); 
    }

    if(mostrarEvolucionFitness){
        cout << endl << endl << "Enfriamiento Simulado Peores " << endl << "ILS_peores=[";
        for(int i=0; i<inicios_fit.size()-1;i++){
            cout << inicios_fit[i] << ", ";
        }
        cout << inicios_fit[inicios_fit.size()-1] <<"]\n";

        cout << endl << "Enfriamiento Simulado Mejores " << endl << "ILS_mejores=[";
        for(int i=0; i<finales_fit.size()-1;i++){
            cout << finales_fit[i] << ", ";
        }
        cout << finales_fit[finales_fit.size()-1] <<"]\n";
    }
    
    return elapsed.count();
}
