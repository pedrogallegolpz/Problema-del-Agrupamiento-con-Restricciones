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




/*
    Pone los clusters de las instancias a -1
*/
void PAR::resetClusters(){
    // Inicializamos los clusters
    for(int i=0; i<num_instancias; i++){
        inst_belong[i] = -1;
    }

    clusters.clear();
    clusters.resize(num_clases);
}




///////////////////
// CONSTRUCTORES
///////////////////

/*
        Pone el num_clases a 0, por lo tanto no hay nada que hacer.
*/
PAR::PAR(){
    num_clases = 0;
    num_atributos = 0;
    num_instancias = 0;
    
   resetClusters();
}

/*
    Inicializa el número de clases y los datos del problema.
*/
PAR::PAR(int num_instancias_source, int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source){
    num_instancias = num_instancias_source;
    num_atributos = num_atributos_source;
    num_clases = num_clases_source;
    instancias = instancias_source;

    centroides.resize(num_clases);
    inst_belong.resize(num_instancias);
    clusters.resize(num_clases);

    for(int i=0; i<num_instancias; i++){
        indices.push_back(i);
    }

    generarRestricciones();
    resetClusters();
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
    inst_belong = par_source.getInstBelong();
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
        cout << "ERROR: Puntos de distintas dimensiones." << endl;
        distancia = 10000000000000;
    }else{
        // Calculamos distancia
        for(int i=0; i<dimension; i++){
            distancia += (p1[i]-p2[i])*(p1[i]-p2[i]);
        }
        distancia = sqrt(distancia);
    }

    return distancia;
}


/*
    Función para calcular el centroide de un conjunto ci
*/
vector<double> PAR::calcularCentroide(int ci){
    int size_c = clusters[ci].size();
    vector<double> mu_i(num_atributos);
    
    // Inicializamos a 0 el centroide
    for(int j=0; j<num_atributos; j++){
        mu_i[j]=0;
    }

    // Hacemos la sumatoria
    for(int j=0; j<num_atributos; j++){
        for(int i=0; i<size_c; i++){
            mu_i[j] += instancias[clusters[ci][i]][j];
        }
        mu_i[j] /= size_c;
    }

    return mu_i;
}


/*
    Calcula la distancia media intra-cluster de un cluster cluster
*/
double PAR::distanciaIntraCluster(int cluster){
    int size_c = clusters[cluster].size();
    double distancia = 0;

    vector<double> centroide = calcularCentroide(cluster);

    // Calculamos la distancia
    for(int i=0; i<size_c; i++){
        distancia += distanciaEntreDosPuntos(instancias[clusters[cluster][i]],centroides[cluster]);
    }   
    distancia /= size_c;

    return distancia;
}


/*
    Calcula la desviación general de la partición C={c1​, c2​,...,cn} definida
    como la media de las desviaciones intra-cluster de cada grupo o cluster
*/
double PAR::desviacionParticion(){
    double distancia = 0;

    for(int c=0; c<num_clases; c++){
        distancia += distanciaIntraCluster(c);
    }
    distancia /= num_clases;

    return distancia;
}


/*
    Calcula el número de restricciones violadas. COMPLETAR
*/
int PAR::infeasibility(){
    int restricciones_violadas=0; 

    for(int i=0; i<ML.size(); i++){
        if( inst_belong[ML[i][0]]!=inst_belong[ML[i][1]] ){     // Incumple una restricción MustLink
            restricciones_violadas++;
        }
    }
    for(int i=0; i<CL.size(); i++){
        if( inst_belong[CL[i][0]]==inst_belong[CL[i][1]]){      // Incumple una restricción CannotLink
            restricciones_violadas++;
        }
    }

    return restricciones_violadas;
}






/*
    Calcula el número de restricciones violadas con una instancia concreta
    antes de ser asignada
        -imstancia  índice directamente sobre el vector de instancias
        -clúster    cluster al que se pretende asignar
*/
int PAR::restriccionesConInstancia(int instancia, int cluster){
    int restricciones_violadas=0;
   
    if(instancia<num_instancias and instancia>=0){
        // Simulamos que la instancia pertenece
        int anterior_cluster = inst_belong[instancia];
        inst_belong[instancia] = cluster;
        
        // Comprobamos las violaciones en el supuesto caso
        for(int j=0; j<num_instancias; j++){
            if(j != instancia){
                if(restricciones[instancia][j]==1 && inst_belong[instancia] != inst_belong[j] && inst_belong[j]!=-1){               // Restricción ML
                    restricciones_violadas++;    
                }else if(restricciones[instancia][j]==-1 && inst_belong[instancia] == inst_belong[j] && inst_belong[j]!=-1){        // Restricción CL
                    restricciones_violadas++;
                }
            }
        }

        // Deshacemos la pertenencia de la instancia al cluster
        inst_belong[instancia]=anterior_cluster;
    
    }else if(instancia<-1){
        restricciones_violadas=num_instancias*num_instancias; 
        cout << "El número de instancia introducido excede los límites" << endl;
    }
    return restricciones_violadas;
}


/*
    Calcula el incremento de restricciones violadas al cambiar una instancia
    de clúster
        -instancia  índice directamente sobre el vector de instancias
        -c1         cluster actual   
        -c2         cluster al que se pretende asignar
*/
int PAR::incrementoInfeasibility(int instancia, int c1, int c2){
    int incremento=1000000;
    if(instancia<num_instancias && instancia>=0){
        incremento = restriccionesConInstancia(instancia,c2) - restriccionesConInstancia(instancia,c1);
    }else{
        cout << "El número de instancia introducido excede los límites" << endl;
    }
    return incremento;
}




/*
    Actualiza los centroides
*/
bool PAR::updateCentroides(){
    bool hay_cambio=false;

    for(int c=0; c<num_clases; c++){    // Para cada centroide
        vector<double> c_old = centroides[c];

        centroides[c] = calcularCentroide(c);

        if(distanciaEntreDosPuntos(centroides[c], c_old) !=0){  // distancia=0 -> es el mismo centroide que antes
            hay_cambio=true;
        }
    }

    return hay_cambio;
}



/*
        Asigna cada instancia a su clúster más cercano teniendo que respetar
        las restricciones fuertes y teniendo en cuenta de que cada instancia
        tiene que escoger el clúster con el que incumpla menos restricciones
        débiles.

        Devuelve un bool que nos dice si el método ha provocado algún cambio
        en los centroides.
    */
bool PAR::asignarInstanciasAClustersCercanos(){
    // Limpiamos la asignación de clusters que hubiese
    for(int c=0; c<num_clases; c++){
        clusters[c].clear();
    }
    
    // PRIMER PASO Asignamos a cada instancia su clúster más cercano
    for(int i=0; i<indices.size(); i++){
        // Comprobamos la restricciones que se han violado
        vector<int> restricciones_violadas (num_clases);

        int menor_infeas = incrementoInfeasibility(indices[i],inst_belong[indices[i]], 0);  
        restricciones_violadas[0]=menor_infeas;      
        for(int c=1; c<num_clases; c++){
            int infeas = incrementoInfeasibility(indices[i],inst_belong[indices[i]], c);
           
            restricciones_violadas[c] = infeas;

            if(infeas < menor_infeas){
                menor_infeas = infeas;
            }
        }
       
        // Cogemos los clúster candidatos y entre ellos elegiremos el más cercano
        vector<int> candidatos;
        for(int j=0; j<restricciones_violadas.size(); j++){
            if(restricciones_violadas[j]==menor_infeas){
                candidatos.push_back(j);
            }
        }
    

        // Comprobamos que centroide está mas cerca a nuestra instancia[indices[i]]
        double menor_distancia = distanciaEntreDosPuntos(instancias[indices[i]],centroides[candidatos[0]]);
        int cluster_cercano = candidatos[0];

        for(int c=1; c<candidatos.size(); c++){

            double distancia_actual = distanciaEntreDosPuntos(instancias[indices[i]],centroides[candidatos[c]]);

            if(distancia_actual < menor_distancia){              
                cluster_cercano = candidatos[c];
            }
        }
        
        // Guardamos la información
        inst_belong[indices[i]] = cluster_cercano;
        clusters[cluster_cercano].push_back(indices[i]);
    }

    // SEGUNDO PASO Comprobamos las restricciones fuertes
    for(int c=0; c<clusters.size(); c++){
        // Si el clúster está vacío buscamos los candidatos para asignarle uno 
        // la condición es que no deje vacío otro clúster
        if(clusters[c].empty()){
            
            vector<int> candidatos;
            for(int i=0; i<num_instancias; i++){
                if(clusters[inst_belong[i]].size()>1){      // El clúster del que le quitaríamos no se quedaría vacío
                    candidatos.push_back(i);
                }
            }
            
            // De los candidatos buscamos el más cercano
            double menor_distancia = distanciaEntreDosPuntos(instancias[candidatos[0]], centroides[c]);
            int instancia_cercana = 0;
            for(int i=1; i<candidatos.size(); i++){
                double distancia_actual = distanciaEntreDosPuntos(instancias[candidatos[i]], centroides[c]);

                if(distancia_actual < menor_distancia){
                    instancia_cercana = candidatos[i];
                }
            }

            // Actualizamos el clúster
            int indice_borrar=-1;
            for(int i=0; i<clusters[inst_belong[instancia_cercana]].size(); i++){
                if(clusters[inst_belong[instancia_cercana]][i] == instancia_cercana){
                    indice_borrar=i;
                    i=clusters[inst_belong[instancia_cercana]].size();       // Salimos del bucle
                }
            }
            clusters[inst_belong[instancia_cercana]].erase(clusters[inst_belong[instancia_cercana]].begin()+indice_borrar);

            inst_belong[instancia_cercana] = c;
            clusters[c].push_back(instancia_cercana);
        }
    }

    //  TERCER PASO Actualizamos centroides
    bool hay_cambio = updateCentroides();

    return hay_cambio;
}




//////////////////////////////////
//    Mostrar Info por Pantalla
//////////////////////////////////


/*
    Nos muestra el estado del problema actual para cada clúster
        -Nº de instancias que pertenecen al clúster
        -Centroide
        -Distancia intra clúster
    Además del problema general como es:
        -Infeasibility
        -Distancia media intra clúster del problema
*/
void PAR::mostrarEstado(){
    cout << "--------------- ESTADO DEL PROBLEMA PAR ---------------" << endl;
    cout << "Nº Clases (clusters):\t\t" << num_clases << endl;
    cout << "Nº de instancias:\t\t" << num_instancias << endl;
    cout << "Nº de atributos por instancia:\t" << num_atributos << endl << endl;

    cout << "Desviación: " << desviacionParticion() << endl;
    cout << "Infeasibility: " << infeasibility() << endl << endl;


    cout << "CLUSTERS ---" << endl;
    for(int c=0; c<num_clases; c++){
        cout << "\tCluster " << c << ":" << endl;
        cout << "\t\tCentroide: (";
        for(int i=0; i<num_atributos; i++){
            if(i<num_atributos-1){
                cout << centroides[c][i] << ", ";
            }else{
                cout << centroides[c][i] << ")" << endl;
            }
        }
        cout << "\t\tNº instancias: " << clusters[c].size() << endl;
        cout << "\t\tDistancia intra-cluster: " << distanciaIntraCluster(c) << endl;
    }
}

/*
    Nos muestra un punto con sus coordenadas
*/
void PAR::mostrarPunto(vector<double> p){
    cout << "(";
    for(int i=0; i<p.size()-1; i++){
        cout << p[i] << ", ";
    }
    cout << p[p.size()-1] << ")" << endl;
}