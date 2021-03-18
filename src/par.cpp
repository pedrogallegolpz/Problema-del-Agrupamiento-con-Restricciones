#include <iostream>
#include <vector>
#include <math.h> 
#include "../include/random.h" 
#include "../include/par.h"


using namespace std;


/////////////////////
// MÉTODOS PRIVADOS
/////////////////////

/*
    Creamos los vectores ML y CL
*/
void PAR::generarRestricciones(){
    for(int i=0; i< num_instancias; i++){
        for(int j=i+1; j<num_instancias; j++){
            if(restricciones[i][j]==1){         // Restricciones ML
                vector<int> v;
                v.push_back(i);
                v.push_back(j);
                ML.push_back(v);
            }else if(restricciones[i][j]==-1){  // Restricciones CL
                vector<int> v;
                v.push_back(i);
                v.push_back(j);
                CL.push_back(v);
            }
        }
    }
}



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
    for(int i=0; i<instancias.size();i++){
        indices.push_back(i);
    }

    generarRestricciones();
}

/*
    Constructor copia
*/
PAR::PAR(const PAR &par_source){
    num_instancias = par_source.getNumInstancias();
    num_atributos = par_source.getNumAtributos();
    num_clases = par_source.getNumClases();
    instancias = par_source.getInstancias();
    centroides = par_source.getCentroides();
    clusters = par_source.getClusters();
    restricciones = par_source.getRestricciones();
    indices = par_source.getIndices();

    generarRestricciones();
}


//////////////////
// MÉTODOS
//////////////////

 /*
    Barajar indices
*/
void PAR::shuffleInstances(){
    indices = ShuffleIndices(num_instancias);
}


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
    Definimos ℎ𝐶(∙) como la función que dada una instancia xi
    devuelve la etiqueta j asociada al cluster cj al que xi pertenece
    según la partición C. getClusterFromInstance hace esta función
*/ 
int getClusterFromInstance(int i){

    // COMPLETARRRRRRRR

    return 0;
}



/*
    Calcula el número de restricciones violadas. COMPLETAR
*/
int PAR::infeasibility(){
    int restricciones_violadas=0; 

    for(int i=0; i<ML.size(); i++){
        if( getClusterFromInstance(ML[i][0])!=getClusterFromInstance(ML[i][1]) ){
            restricciones_violadas++;
        }
    }
    for(int i=0; i<CL.size(); i++){
        if( getClusterFromInstance(CL[i][0])==getClusterFromInstance(CL[i][1]) ){
            restricciones_violadas++;
        }
    }

    return restricciones_violadas;
}


/*
    Asigna instancia al cluster más cercano y que cumpla las restricciones
*/
void PAR::asignarInstanciasAClustersCercanos(){
    for(int i=0; i<instancias.size(); i++){
        vector<double> distancias;      // distancias a centroides

        for(int c=0; c<centroides.size(); c++){
            int dist = distanciaEntreDosPuntos(instancias[i],centroides[c]);
            distancias.push_back(dist);   
        }

        //COMPLETARRRRRRRR

        vector<int> restricciones_incumplidas;       // restricciones violadas con los cluster
        //ACORDARME DE LIMPIAR EL VECTOR DE DISTANCIAS
    }
}