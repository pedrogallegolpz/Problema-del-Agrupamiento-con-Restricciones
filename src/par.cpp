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
    GREATER_DIST = 0;
    num_clases = 0;
    num_atributos = 0;
    num_instancias = 0;
    SEED = 0;
    funcion_objetivo = 10000000;
    iterations_BL = 0;
    
    resetClusters();
}

/*
    Inicializa el número de clases y los datos del problema.
*/
PAR::PAR(int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source, vector<vector<double>> restricciones_source, int seed_source){
    setNumAtributos(num_atributos_source);
    setNumClases(num_clases_source);
    setInstancias(instancias_source);
    setRestricciones(restricciones_source);
    setSeed(seed_source);
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
    GREATER_DIST = par_source.getGreaterDist();
    SEED = par_source.getSeed();

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
    Actualiza los centroides
*/
bool PAR::updateCentroides(){
    bool hay_cambio=false;

    vector<vector<double>> c_old (centroides);

    for(int c=0; c<num_clases; c++){    // Para cada centroide
        centroides[c] = calcularCentroide(c);

        // Comprobamos si el nuevo centroide ha cambiado
        for(int i=0; i<centroides[c].size() and !hay_cambio; i++){
            if(centroides[c][i]!= c_old[c][i]){
                hay_cambio=true;
            }
        }
    }

    return hay_cambio;
}






/*
    Calcula el número de restricciones violadas con una instancia concreta
    antes de ser asignada
        -instancia  índice directamente sobre el vector de instancias
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






////////////////////////////////////////////////////////////////
//      Métodos principales para las distintas Metaheurísticas
////////////////////////////////////////////////////////////////

/*
    USADA PARA LA METAHEURÍSTICA GREEDY COPKM PARA CADA ITERACIÓN

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



/*
    USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

    Asigna clústers a las instancia de forma aleatoria respetando
    las restricciones fuertes del problema.

    Devuelve true si ha sido exitosa la operación. 
    En otro caso devuelve false.
*/
bool PAR::asignarInstanciasAleatoriamente(){
    bool exito_inicializando=true;      // Nos dirá si se queda algún clúter vacío

    // Reseteamos la información que haya
    resetClusters();

    // Asignamos a cada instancia un clúster aleatorio sin tener en cuenta restricciones
    for(int i=0; i<num_instancias; i++){
        // Generamos el cluster aleatorio
        int cluster_aleatorio = Randint(0, num_clases-1);

        // Asignamos el cluster a la instancia i
        inst_belong[i] = cluster_aleatorio;
        clusters[cluster_aleatorio].push_back(i);
    }

    // Arreglamos las restricciones fuertes que se incumplan
    shuffleInstances();
    for(int c=0; c<num_clases and exito_inicializando; c++){    // Para cada clúster comprobamos que no esté vacío

        if(clusters[c].size() == 0){    // Si está vacío
            exito_inicializando = false;
            for(int i=0; i<indices.size(); i++){    // Para cada instancia comprobamos si se puede cambiar
                
                int c_old = inst_belong[indices[i]];
                if(clusters[c_old].size()>1){              // Si se puede cambiar cambiamos

                    // Hacemos la modificación
                    inst_belong[indices[i]]=c;      // Añadimos al nuevo cluster
                    clusters[c].push_back(i);
                    for(int i=0; i<clusters[c_old].size(); i++){
                        if(clusters[c_old][i]==indices[i]){
                            clusters[c_old].erase(clusters[c_old].begin()+i);   // eliminamos del anterior cluster
                        }
                    }
                    exito_inicializando = true;
                }
            }
        }
    }
    
    if(!exito_inicializando){
        cout << "Error al asignar instancias aleatoriamente: No se ha conseguido hacer respetando todas las restricciones fuertes." << endl;
    }


    // Actualizamos los centroides y función objetivo
    updateCentroides();
    fitnessFunction();

    return exito_inicializando;
}



/*
    USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

    Nos devuelve verdadero si cambia el cluster. Solo hace el cambio si mejora
    la solución al problema.
*/
bool PAR::cambioClusterMejor(int instancia, int cluster){
    // Probamos el cambio
    int c_old = inst_belong[instancia];     // Guardamos los datos anteriores
    double fitness_old = funcion_objetivo;


    inst_belong[instancia]=cluster;         // cambiamos
    clusters[cluster].push_back(instancia);
    for(int i=0; i<clusters[c_old].size(); i++){
        if(clusters[c_old][i]==instancia){
            clusters[c_old].erase(clusters[c_old].begin()+i);
        }
    }
    updateCentroides();
    

    bool mejora=true;
    if(fitnessFunction() >= fitness_old){ 
        mejora=false;

        // Recuperamos los datos que teníamos anteriormente
        funcion_objetivo = fitness_old;
        
        inst_belong[instancia]=c_old;                       // asignamos
        clusters[c_old].push_back(instancia);

        for(int i=0; i<clusters[cluster].size(); i++){      // borramos el cambio
            if(clusters[cluster][i]==instancia){
                clusters[cluster].erase(clusters[cluster].begin()+i);
            }
        }

        updateCentroides();


    }
    
    iterations_BL++;

    
    return mejora;
}


/*
    USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

    Nos devuelve True en caso de haber encontrado un vecino mejor. En otro caso
    estamos ante la solución óptima y nos dará False
*/
bool PAR::buscarPrimerVecinoMejor(){
    
    bool hay_cambio=false;          // Nos dice si hemos encontrado un vecino mejor

    // Generamos el vecindario virtual:
    // Un vecino virtual tendrá la forma {i, c} donde
    //      -i      instancia a reasignar clúster
    //      -c      nuevo clúster
    vector<vector<int>> vec_virtual;
    for(int i=0; i<num_instancias; i++){    // para cada instancia
        for(int c=0; c<num_clases; c++){    // para cada clúster
            // Comprobamos que el clúster nuevo no es el actual de la instancia
            // y que no dejamos vacío el clúster antiguo
            if(inst_belong[i]!=c && clusters[inst_belong[i]].size()>1){
                vector<int> vecino;
                vecino.push_back(i);
                vecino.push_back(c);
                vec_virtual.push_back(vecino);
            }
        }
    }

    vector<int> ind_vec = ShuffleIndices(vec_virtual.size());      // Metemos aleatoriedad

    for(int i=0; i<ind_vec.size() and !hay_cambio and iterations_BL<=100000; i++){
        hay_cambio = cambioClusterMejor(vec_virtual[ind_vec[i]][0], vec_virtual[ind_vec[i]][1]);
    }

    if(iterations_BL>=100000){
        hay_cambio = false;     // Forzamos salir
    }

    return hay_cambio;
}




    
/////////////////////////////////////////////////////////////
//    Métodos que nos dicen cómo de buena es la solución
/////////////////////////////////////////////////////////////


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
    Función objetivo. Será la función que buscaremos minimizar y nos dirá 
    cómo de buena es nuestra solución actual
*/
double  PAR::fitnessFunction(){
    /*
        Se escoge el lamda como [D]/|R| donde:
            -[D]   es la mayor distancia que existe entre dos instancias
            -|R|   es el número de restricciones totales que hay

        para coger una especie de frecuencia relativa haciendo que ambas
        medidas ponderen de igual forma a nuestra función objetivo
    */

    double lambda = GREATER_DIST / (ML.size() + CL.size());

    funcion_objetivo = desviacionParticion()+ lambda*infeasibility();

    return funcion_objetivo;
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
    cout << "\n\n--------------------------- ESTADO DEL PROBLEMA PAR ---------------------------" << endl << endl;
    cout << "Nº Clases (clusters):\t\t" << num_clases << endl;
    cout << "Nº de instancias:\t\t" << num_instancias << endl;
    cout << "Nº de atributos por instancia:\t" << num_atributos << endl << endl;

    cout << "Desviación:\t\t" << desviacionParticion() << endl;
    cout << "Infeasibility:\t\t" << infeasibility() << endl;
    cout << "Función objetivo:\t" << fitnessFunction() << endl << endl;


    cout << "CLUSTERS ---" << endl;
    for(int c=0; c<num_clases; c++){
        cout << "\tCluster " << c << ":" << endl;
        cout << "\t\tCentroide: ";
        mostrarPunto(centroides[c]);
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