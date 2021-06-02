#include <iostream>
#include <vector>
#include <math.h>  
#include <cmath>
#include <chrono>
#include "../include/random.h"
#include "../include/enfriamientosimulado.h"

using namespace std;

/*
    Genera el vector de temperaturas
        - Ti: temperatura inicial
        - Tf: temperatura final
        - M:  número de enfriamientos a realizar

    Devuelve el vector de temperaturas
*/
vector<double> generarTemperaturas(double Ti, double Tf, int M){
    double beta = (Ti-Tf)/(M*Ti*Tf);
    vector<double> temperatura;

    // Generamos los M enfriamientos (M+1 temperaturas)
    temperatura.push_back(Ti);
    
    double k = pow(Tf/Ti, 1/M);
    
    for(int i=0; i<M; i++){
       //temperatura.push_back( k*temperatura[i] ); // T <- k*T
       temperatura.push_back( temperatura[i]/(1+beta*temperatura[i]) );
    }
    
    return temperatura;
}

/*
    Ejecuta el algoritmo de enfriamiento simulado en un problema PAR

    Devuelve el tiempo que ha tardado en ejecutarse en milisegundos

        -mostrarEstado: muestra el estado del problema al terminar el algoritmo
        -mostrarEvolucionFitness:   muestra en cada generación el valor de la función objetivo
*/
int enfriamientoSimulado(PAR &par, int seed, bool mostrarEstado, bool mostrarEvolucionFitness){
    // Asignamos semilla aleatoria
    Set_random(seed);

    // Limpiamos el problema
    par.clear();
    par.setIterationsFF(0);

    // Inicializamos constantes:
    double mu = 0.3;     // Se admiten soluciones de hasta 'mu' tanto por ciento de empeoramiento
    double phi = 0.3;    // Se aceptan las soluciones admitidas con probabilidad 'phi'
    int max_vecinos = 10*par.getNumInstancias(); // max_vecinos generados en cada iteracion
    int max_exitos = 0.1*max_vecinos;
    
    int valoraciones_funcion_objetivo = 100000;  

    
    auto begin = chrono::high_resolution_clock::now();
    // Inicializamos aleatoriamente la asignación de clústers
    if(!par.crearSolucionAleatoria()){
        cout << "Error al asignar instancias aleatoriamente en el algoritmo de enfriamiento simulado." << endl;
    }

    // Generamos las temperaturas
    double Ti = (mu*par.fitnessFunction())/(-log(phi));
    double Tf = 0.001;

    // Nos aseguramos de que Ti sea menor que Tf
    if(Ti<=Tf){
        Tf=Ti*Tf;
    }
    
    int M = 100000/max_vecinos;
    vector<double> temperaturas = generarTemperaturas( Ti, Tf, M);

    // Ejecutamos ES
    int iteraciones=0;
    bool para=false;
    while(par.getIterationsFF()<valoraciones_funcion_objetivo && iteraciones<M+1 && para==false){
        para = par.enfriamientoSimulado(temperaturas[iteraciones], max_vecinos, max_exitos);
        iteraciones++;
    }
    
    // Cogemos la mejor solución
    par.simularMejorSolucion();
        
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    string name = "ES";
    
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
        // Evolución funcion objetivo
        vector<double> mejores_fit = par.getMejoresFitnessTrayectoria();
        vector<double> peores_fit = par.getPeoresFitnessTrayectoria();

        cout << endl << "Enfriamiento Simulado Mejores " << endl << "ES_mejores=[";
        for(int i=0; i<mejores_fit.size()-1;i++){
            cout << mejores_fit[i] << ", ";
        }
        cout << mejores_fit[mejores_fit.size()-1] <<"]\n";


        cout << endl << endl << "Enfriamiento Simulado Peores " << endl << "ES_mejores=[";
        for(int i=0; i<peores_fit.size()-1;i++){
            cout << peores_fit[i] << ", ";
        }
        cout << peores_fit[peores_fit.size()-1] <<"]\n";
    }
    
    return elapsed.count();
}
