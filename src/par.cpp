#include <iostream>
#include <vector>
#include <math.h>  
#include "../include/par.h"


using namespace std;


///////////////////
// CONSTRUCTORES
///////////////////

/*
        Pone el num_clases a 0, por lo tanto no hay nada que hacer.
*/
PAR::PAR(){
    num_clases = 0;
}

/*
    Inicializa el número de clases y los datos del problema.
*/
PAR::PAR(int num_instancias_source, int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source){
    num_instancias = num_instancias_source;
    num_atributos = num_atributos_source;
    num_clases = num_clases_source;
    instancias = instancias_source;
}

/*
    Constructor copia
*/
PAR::PAR(const PAR &cluster_source){
    num_instancias = cluster_source.getNumInstancias();
    num_atributos = cluster_source.getNumAtributos();
    num_clases = cluster_source.getNumClases();
    instancias = cluster_source.getInstancias();
    centroides = cluster_source.getCentroides();
    clusters = cluster_source.getClusters();
    restricciones = cluster_source.getRestricciones();
}


//////////////////
// MÉTODOS
//////////////////

/*
    Calcula la distancia euclídea entre dos puntos de Rn
*/
double PAR::distanciaEntreDosPuntos(vector<double> p1, vector<double> p2){
    int dimension = p1.size();
    int dimension_verificar = p2.size();
    double distancia = 0;

    // Verificamos que son vectores de la misma dimensión
    if(dimension!=dimension_verificar){
        cout << "Puntos de distintas dimensiones, salida distancia negativa";
        distancia = -1.0;
    }else{
        // Calculamos distancia
        for(int i = 0; i<dimension; i++){
            distancia += (p1[i]-p2[i])*(p1[i]-p2[i]);
        }
        distancia = sqrt(distancia);
    }

    return distancia;
}

/*
    Función para calcular el centroide de un conjunto ci
*/
vector<double> PAR::calcularCentroide(vector<vector<double>> ci){
    int size_c = ci.size();
    int size_xj = ci[0].size();
    vector<double> mu_i(size_xj);
    
    // Inicializamos a 0 el centroide
    for(int j=0; j<size_xj; j++){
        mu_i[j]=0;
    }

    // Hacemos la sumatoria
    for(int j=0; j<size_xj; j++){
        for(int i=0; i<size_c; i++){
            mu_i[j] += ci[i][j];

            // Dividimos entre |ci| una vez hayamos sumado todos
            if(i == size_c-1){
                mu_i[j] /= size_c;
            }
        }
    }

    return mu_i;
}


/*
    Calcula la distancia media intra-cluster de un cluster ci
*/
double PAR::distanciaIntraCluster(vector<vector<double>> ci){
    int size_c = ci.size();
    double distancia = 0;

    vector<double> centroide = calcularCentroide(ci);

    // Calculamos la distancia
    for(int i=0; i<size_c; i++){
        distancia += distanciaEntreDosPuntos(ci[i],centroide);
    }   
    distancia /= size_c;

    return distancia;
}


/*
    Calcula la desviación general de la partición C={c1​, c2​,...,cn} definida
    como la media de las desviaciones intra-cluster de cada grupo o cluster
*/
double PAR::desviacionParticiom(vector<vector<vector<double>>> C){
    int size_c = C.size();
    double distancia = 0;

    for(int i=0; i<size_c; i++){
        distancia += distanciaIntraCluster(C[i]);
    }

    return distancia;
}


/*
        Calcula el número de restricciones violadas. COMPLETAR
    */
    int PAR::infeasibility(){
        int restricciones_violadas=0; 

        return restricciones_violadas;
    }