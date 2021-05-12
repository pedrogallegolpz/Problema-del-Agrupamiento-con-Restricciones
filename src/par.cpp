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


/*
    Añade a un elemento a 'poblacion' ordenada según la función
    objetivo de cada solución (posición 0 la mejor). Si 'pob'
    tiene ya TAM_POBLACION elementos entonces borramos el peor.
*/
void PAR::insertPoblacion(vector<double> &elemento, vector<vector<double>> &pob){
    // Si la población está vacía añadimos

    if(pob.empty() && TAM_POBLACION>0){
        pob.push_back(elemento);

    }else if(TAM_POBLACION>0){
        double movimiento = ((double)(pob.size()-1))/2;
        double elemento_medio = movimiento;
        bool insertado = false;

        // Insertamos de forma ordenada
        while(!insertado){
            int it = elemento_medio;

            if(movimiento<1){
                // Comparamos las funciones objetivo
                if(elemento[elemento.size()-1] < pob[it][pob[it].size()-1]){
                    // Recorremos hacia la izquierda hasta encontrar uno que elemento se mayor que él
                    for(int i=it; i>=0 && !insertado; i--){
                        if(elemento[elemento.size()-1] > pob[i][pob[i].size()-1]){
                            pob.insert(pob.begin()+i+1, elemento);
                            insertado=true;
                        }
                    }
                    // Si no se ha insertado es porque es el más pequeño
                    if(!insertado){
                        pob.insert(pob.begin(),elemento);
                        insertado=true;
                    }
                }else if(elemento[elemento.size()-1] > pob[it][pob[it].size()-1]){
                    // Recorremos hacia la derecha hasta encontrar uno que elemento sea menor que el
                    for(int i=it; i<pob.size() && !insertado; i++){
                        if(elemento[elemento.size()-1] <= pob[i][pob[i].size()-1]){
                            pob.insert(pob.begin()+i, elemento);
                            insertado=true;
                        }
                    } 
                    // Si no se ha insertado es porque es el más grande
                    if(!insertado){
                        pob.insert(pob.end(),elemento);
                        insertado=true;
                    }         
                }else{
                    pob.insert(pob.begin()+it,elemento);
                    insertado=true;               
                }            
            }else{
                movimiento = movimiento/2;  // Lo que nos vamos a desplazar para encontrar la posición

                // Comparamos las funciones objetivo
                if(elemento[elemento.size()-1] < pob[it][pob[it].size()-1]){
                    elemento_medio -= movimiento;
                }else if(elemento[elemento.size()-1] > pob[it][pob[it].size()-1]){
                    elemento_medio += movimiento;
                }else{
                    pob.insert(pob.begin()+it,elemento);
                    insertado=true; 
                }
            }
        }
        if(pob.size()>TAM_POBLACION){
            pob.erase(pob.end()-1);
        }

    }else{
        cout << "ERROR: vector poblacion can only be empty becouse TAM_POBLACION=0" << endl;
    }
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
    iterations_ff = 0;
    TAM_POBLACION = 0;
    epoca = 0;
    
    resetClusters();
}

/*
    Inicializa el número de clases y los datos del problema.
*/
PAR::PAR(int num_atributos_source, int num_clases_source, vector<vector<double>> instancias_source, vector<vector<double>> restricciones_source, int seed_source, int tam_pob){
    setNumAtributos(num_atributos_source);
    setNumClases(num_clases_source);
    setInstancias(instancias_source);
    setRestricciones(restricciones_source);
    setSeed(seed_source);
    setTamPoblacion(tam_pob);
    setEpoca(0);
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
    TAM_POBLACION = par_source.getTamPoblacion();
    epoca = par_source.getEpoca();

    generarRestricciones();
}


/*
    Limpia todos los cálculos hechos
*/
bool PAR::clear(){
    necesidad_actualizar_centroides = false;
    funcion_objetivo =100000;
    iterations_ff=0;
    resetClusters();
    poblacion.clear();
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
        cout << "ERROR: Puntos de distintas dimensiones: p1.dim = " << p1.size() << ", p2.dim="<< p2.size() << endl;
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


/*
    Crea solución aleatoria y la devuelve
*/
bool PAR::asignarInstanciasAleatoriamente(vector<int> &solucion){
    bool exito_inicializando=true;      // Nos dirá si se queda algún clúter vacío


    // Reseteamos la información que haya
    resetClusters();

    // Asignamos a cada instancia un clúster aleatorio sin tener en cuenta restricciones
    for(int i=0; i<num_instancias; i++){
        // Generamos el cluster aleatorio
        int cluster_aleatorio = Randint(0, num_clases-1);

        // Asignamos el cluster a la instancia i
        solucion[i] = cluster_aleatorio;
        clusters[cluster_aleatorio].push_back(i);
    }

    // Arreglamos las restricciones fuertes que se incumplan
    shuffleInstances();
    for(int c=0; c<num_clases and exito_inicializando; c++){    // Para cada clúster comprobamos que no esté vacío

        if(clusters[c].size() == 0){    // Si está vacío
            exito_inicializando = false;
            for(int i=0; i<indices.size(); i++){    // Para cada instancia comprobamos si se puede cambiar
                
                int c_old = solucion[indices[i]];
                if(clusters[c_old].size()>1){              // Si se puede cambiar cambiamos
                    necesidad_actualizar_centroides=true;

                    // Hacemos la modificación
                    solucion[indices[i]]=c;      // Añadimos al nuevo cluster
                    clusters[c].push_back(indices[i]);
                    for(int k=0; k<clusters[c_old].size(); k++){
                        if(clusters[c_old][k]==indices[i]){
                            clusters[c_old].erase(clusters[c_old].begin()+k);   // eliminamos del anterior cluster
                        }
                    }
                    exito_inicializando = true;
                }
            }
        }
    }
    return exito_inicializando;
}


/*
    Simula una solución. Actualiza clústers, centroides y función objetivo
*/
bool PAR::simularSolucion(vector<double> &solucion){
    bool exito=true;

    inst_belong.resize(num_instancias);
    resetClusters();
    for(int i=0; i<num_instancias; i++){
        inst_belong[i]=solucion[i];
        clusters[solucion[i]].push_back(i);
    }
    int num=0;
    
    // Arreglamos las restricciones fuertes que se incumplan
    shuffleInstances();
    for(int c=0; c<num_clases and exito; c++){    // Para cada clúster comprobamos que no esté vacío

        if(clusters[c].size() == 0){    // Si está vacío
            exito = false;
            for(int i=0; i<indices.size(); i++){    // Para cada instancia comprobamos si se puede cambiar
                
                int c_old = inst_belong[indices[i]];
                if(clusters[c_old].size()>1){              // Si se puede cambiar cambiamos
                    necesidad_actualizar_centroides=true;

                    // Hacemos la modificación
                    inst_belong[indices[i]]=c;      // Añadimos al nuevo cluster
                    solucion[indices[i]]=c;         // Actualizamos el vector original
                    clusters[c].push_back(indices[i]);
                    for(int k=0; k<clusters[c_old].size(); k++){
                        if(clusters[c_old][k]==indices[i]){
                            clusters[c_old].erase(clusters[c_old].begin()+k);   // eliminamos del anterior cluster
                        }
                    }
                    exito = true;
                }
            }
        }
    }
    

    updateCentroides();
    necesidad_actualizar_centroides=false;
    
    fitnessFunction();

    return exito;
}


////////////////////////////////////////////////////////////////
//
//      Métodos principales para las distintas Metaheurísticas
//
////////////////////////////////////////////////////////////////


/////////////////////
//  GREEDY
/////////////////////
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
    necesidad_actualizar_centroides=false;

    return hay_cambio;
}


/////////////////////
//  BÚSQUEDA LOCAL
/////////////////////
/*
    USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL EL PRIMERO MEJOR

    Asigna clústers a las instancia de forma aleatoria respetando
    las restricciones fuertes del problema.

    Devuelve true si ha sido exitosa la operación. 
    En otro caso devuelve false.
*/
bool PAR::crearSolucionAleatoria(){
    bool exito_inicializando = asignarInstanciasAleatoriamente(inst_belong);      // Nos dirá si se queda algún clúter vacío    
    
    if(!exito_inicializando){
        cout << "Error al asignar instancias aleatoriamente: No se ha conseguido hacer respetando todas las restricciones fuertes." << endl;
    }

    // Actualizamos los centroides y función objetivo
    if(necesidad_actualizar_centroides){
        updateCentroides();
        necesidad_actualizar_centroides=false;
    }
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
    necesidad_actualizar_centroides=false;
    

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
        necesidad_actualizar_centroides=false;


    }
    
    iterations_ff++;

    
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

    for(int i=0; i<ind_vec.size() and !hay_cambio and iterations_ff<=100000; i++){
        hay_cambio = cambioClusterMejor(vec_virtual[ind_vec[i]][0], vec_virtual[ind_vec[i]][1]);
    }


    return hay_cambio and iterations_ff<100000;
}


/*
    USADO PARA LA METAHEURÍSTICA BÚSQUEDA LOCAL SUAVE Y MEMÉTICO 

    Escoge el mejor vecino y actualiza la solución. Repite el proceso tantas veces como
    #fallos en encontrar mejor vecino se produzcan, si no mejora o si recorremos todas
    las instancias.
    Actúa sobre la solución pasada por parámetro. Se permiten #fallos en el algoritmo

    Nos devuelve True en caso de haber encontrado un vecino mejor. En otro caso
    estamos ante la solución óptima y nos dará False
*/
bool PAR::busquedaLocalSuave(vector<double> &solucion, int fallos){
    bool exito = true;

    shuffleInstances();
    
    // Simulamos la solución de primeras
    simularSolucion(solucion);
    solucion[solucion.size()-2]=epoca;
    solucion[solucion.size()-1]=funcion_objetivo;
    iterations_ff++;

    int f = 0;  // Cuenta de fallos
    bool mejora = true;
    int i = 0;
    vector<double> mejor_solucion=solucion;
    
    while((mejora || f<fallos) && i<num_instancias){
        mejora = false;

        int cluster_de_partida = solucion[indices[i]];   
        int mejor_cluster = cluster_de_partida;
        double f_objetivo_mejor = mejor_solucion[mejor_solucion.size()-1];

        // Si podemos sacarlo de ese cluster 
        if(clusters[solucion[indices[i]]].size()>1){

            // Buscamos el mejor clúster
            for(int c=0; c<num_clases; c++){
                if(c!=cluster_de_partida){
                    solucion[indices[i]]=c;

                    // Simulamos
                    simularSolucion(solucion);
                    solucion[solucion.size()-2]=epoca;
                    solucion[solucion.size()-1]=funcion_objetivo;

                    // Actualizamos iteraciones
                    iterations_ff++;

                    // Si mejoran los resultados
                    if(funcion_objetivo < f_objetivo_mejor){
                        mejor_cluster = c;
                        f_objetivo_mejor = funcion_objetivo;
                        mejor_solucion = solucion;
                    }
                }
            }

            // Nos quedamos la mejor solución
            solucion = mejor_solucion;

            // Actualizamos las variables dependiendo si se ha mejorado la solución en esta iteración o no
            if(mejor_cluster!=cluster_de_partida){
                mejora=true;
            }else{
                f++;
            }
        }
        // Avanzamos con el índice
        i++;
    }

    return exito;
}




////////////////////////////
//  GENÉTICOS Y MEMÉTICOS
////////////////////////////
/*
    USADO PARA LA METAHEURÍSTICA GENÉTICA

    Crea un conjunto de TAM_POBLACION soluciones y se guardan en poblacion

    Devuelve true si ha sido exitosa la operación. 
    En otro caso devuelve false.
*/
bool PAR::crearPoblacionAleatoria(){
    bool exito=true;
    recorrido_fun_objetivo.clear();

    for(int i=0; i<TAM_POBLACION && exito; i++){
        // Creamos solución
        exito = crearSolucionAleatoria();


        // Transcribimos la solución en términos de elementos de 'poblacion'
        vector<double> aux;
        for(int j=0; j<inst_belong.size(); j++){
            aux.push_back(inst_belong[j]);
        }
        aux.push_back(epoca);
        aux.push_back(funcion_objetivo);


        
        // Guardamos en la poblacion
        insertPoblacion(aux, poblacion);
    }

    iterations_ff+=50;
    
    return exito;
}

 /*
    USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

    Lleva a cabo el operador de cruce uniforme. Se pasan por referencia los dos padres
    y el vector resultante.

    Devuelve true si ha sido exitosa la operación.
    En otro caso devuelve false.
*/
bool PAR::operadorCruceUniforme(vector<double> &padre1, vector<double> &padre2, vector<double> &hijo){
    vector<int> inds = ShuffleIndices(num_instancias);

    hijo.resize(num_instancias);
    for(int i=0;i<num_instancias;i++){
        if(i<num_instancias/2){
            hijo[inds[i]]=padre1[inds[i]];
        }else{
            hijo[inds[i]]=padre2[inds[i]];
        }
    }
    bool exito = simularSolucion(hijo);
    hijo.push_back(epoca);
    hijo.push_back(funcion_objetivo);

    // Actualizamos iteraciones
    iterations_ff++;

    return exito;
}


/*
    USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

    Lleva a cabo el operador de cruce por segmento fijo. Se pasan por referencia los dos
    padres y el vector resultante.

    Devuelve true si ha sido exitosa la operación.
    En otro caso devuelve false.
*/
bool PAR::operadorCruceSegmentoFijo(vector<double> &padre1, vector<double> &padre2, vector<double> &hijo){
    // Generamos el tamaño y la posición inicial
    int tam = Randint(1,num_instancias);
    int pos_ini = Randint(0,num_instancias-1);

    // Generamos los índices para el cruce uniforme
    vector<int> indx = ShuffleIndices(num_instancias-tam);

    hijo.resize(num_instancias);

    // Cruzamos
    // Asignamos al padre1
    for(int i=0; i<tam; i++){
        int pos = (i+pos_ini)%num_instancias;
        hijo[pos] = padre1[pos];
    }

    // Asignamos uniformemente
    for(int i=0; i<num_instancias-tam;i++){
        int pos = (i+tam+pos_ini)%num_instancias;
        if(i<(num_instancias-tam)/2){
            hijo[pos] = padre1[pos];
        }else{
            hijo[pos] = padre2[pos];
        }
    }


    bool exito = simularSolucion(hijo);
    hijo.push_back(epoca);
    hijo.push_back(funcion_objetivo);

    // Actualizamos iteraciones
    iterations_ff++;

    return exito;
}


/*
    USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

    Lleva a cabo el operador de mutación. Se pasan por referencia la población.
    Además se pasará el número de genes a modificar.

    Devuelve true si ha sido exitosa la operación.
    En otro caso devuelve false.
*/
bool PAR::operadorMutacion(vector<vector<double>> &pob, double ngenes){
    bool exito = true;
    int total_genes = pob.size()*num_instancias;
    bool mutacion_en_esta_epoca = false;
    int epoca_mutacion=1;
    int num_genes_a_mutar = ngenes;
    
    
    if(ngenes<1 && ngenes>0){
        epoca_mutacion = 1/ngenes;
    }

    if(epoca%epoca_mutacion==0){
        mutacion_en_esta_epoca=true;
    }    
        
    if(mutacion_en_esta_epoca){
        for(int it=0; it<ngenes; it++){
           
            int gen = Randint(0, total_genes-1);
            int i = gen/num_instancias;
            int j = gen%num_instancias;
            
            int old_cluster = pob[i][j];
            do{
                // Mutamos el gen[i][j]
                int new_cluster = Randint(0,num_clases-2);

                if(new_cluster >= old_cluster){
                    new_cluster++;
                }

                // Asignamos la mutación
                pob[i][j]=new_cluster;
                bool exito = simularSolucion(pob[i]);

                // Actualizamos iteraciones
                iterations_ff++;
                
    
            }while(pob[i][j]==old_cluster);           

            // Actualizamos función fitness
            pob[i][pob[i].size()-1]=funcion_objetivo;
        }
    }

}


/*
    USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

    Se contemplan las metaheurísticas genéticas tanto estacionarias como generacional
    y los operadores de cruce 'uniforme' y 'segmento fijo'. Ejecuta una época completa
    sobre la población

    params:
        -tipo:      entero que determina si el algoritmo es estacionario(1) o generacional(2)
        -cruce:     entero que determina el tipo de cruce: 'uniforme'(1) o 'segmento fijo'(2)    
        -bls:       entero que determina que cada #bls épocas se haga una BúsquedaLocalSuave
                    si es 0 se ejecutará una época conforme a un algoritmo genético. Si es >0
                    se ejecutará una época conforme a un algoritmo memético.  
        -prob:      probabilidad de que se modifique un cromosoma (bls>0)
        -best:      booleano: se cogen solo los prob*tam mejores si true (bls>0 and prob!=1)
    Nos devuelve True si se ha realizado con éxito el proceso de búsqueda de solución.
    Nos devuelve False en caso contrario.
*/
bool PAR::runEpoch(int tipo, int cruce, int bls, double prob, bool best){

    // Si no hay población hacemos un inicio aleatorio de población de tamaño TAM_POBLACION
    if(poblacion.size()<TAM_POBLACION){
        bool poblacion_creada = crearPoblacionAleatoria();  
        if(!poblacion_creada){
            cout << "ERROR at PAR::runGenetico. 'poblacion' doesn't create."<< endl;
        }
    }

    // Añadimos el mejor fitness al recorrido
    recorrido_fun_objetivo.push_back(poblacion[0][poblacion[0].size()-1]);

    int tam_pob;
    if(tipo == 1){
        // Estacionario
        tam_pob = 2;
    }else{
        // Generacional
        tam_pob = TAM_POBLACION;
    }

    
    // EJECUTAMOS UNA ÉPOCA
    
    // Proceso de selección de Padres
    vector<vector<double>> poblacion_padres = vector<vector<double>>();
    for(int i=0; i<tam_pob; i++){
        int mejorrand = Randint(0,TAM_POBLACION-1);
        int rand2 = Randint(0,TAM_POBLACION-1);
        
        // Me quedo con el mejor
        if(rand2 <= mejorrand){
            mejorrand = rand2;
        }
        
        poblacion_padres.push_back(poblacion[mejorrand]);
    }


    
    // Proceso de cruce
    vector<vector<double>> poblacion_hijos;
    int padres_a_cruzar = 36; // 50*PROB_CRUCE=50*0.7
    if(tipo == 1){
        padres_a_cruzar = 2;  
    }

    for(int it1=0; it1<padres_a_cruzar/2; it1++){
        for(int it2=0; it2<2; it2++){
            vector<double> hijo;
            if(cruce==1){
                // Cruce uniforme
                operadorCruceUniforme(poblacion_padres[2*it1], poblacion_padres[2*it1+1], hijo);
            }else{
                // Cruce segmento fijo
                operadorCruceSegmentoFijo(poblacion_padres[2*it1], poblacion_padres[2*it1+1], hijo);
            }
            // Añadimos a la población
            poblacion_hijos.push_back(hijo);
        }
    }

   
    if(tipo != 1){   // Si es de tipo generacional metemos el resto de la población (no se han cruzado)

        for(int it1=padres_a_cruzar; it1<tam_pob; it1++){
            poblacion_hijos.push_back(poblacion_padres[it1]);
        }
    }
    
    // Proceso mutación 
    double num_total_genes = tam_pob * num_instancias;
    double genes_a_mutar = PROB_MUT_GEN*tam_pob;
    

    operadorMutacion(poblacion_hijos, genes_a_mutar);

   
    if(bls>0 && epoca%bls==0){      // Si hay que aplicar Búsqueda Local Suave en esta época

        int num_soluciones_bls = prob * tam_pob;
        int fallos = num_instancias*0.1;

        // Si hay que coger los mejores
        vector<vector<double>> poblacion_aux;
        if(best && prob<1){
            for(int i=0; i<poblacion_hijos.size(); i++){
                insertPoblacion(poblacion_hijos[i], poblacion_aux);
            }
            poblacion_hijos=poblacion_aux;
        }
        

        // Ejecutamos BLS
        for(int i=0; i<num_soluciones_bls; i++){
            busquedaLocalSuave(poblacion_hijos[i], fallos);

            //Vamos a probar con la BL de la P1
            /*simularSolucion(poblacion_hijos[i]);
            for(int n=0; n<num_instancias; n++){
                buscarPrimerVecinoMejor();
            }
            for(int n=0; n<num_instancias; n++){
                poblacion_hijos[i][n]=inst_belong[n];
            }
            poblacion_hijos[i][poblacion_hijos[i].size()-2]=epoca;
            poblacion_hijos[i][poblacion_hijos[i].size()-1]=funcion_objetivo;
            */
        }
    }

    

    // Reemplazamiento
    if(tipo==1){
        // Estacionario
        for(int i=0; i<poblacion_hijos.size(); i++){
            insertPoblacion(poblacion_hijos[i], poblacion);
        }
    }else{
        // Generacional
        vector<double> mejor_padre = poblacion[0];
        poblacion = vector<vector<double>> ();
        
        for(int i=0; i<poblacion_hijos.size(); i++){
            insertPoblacion(poblacion_hijos[i], poblacion);
        }
        poblacion.erase(poblacion.end()-poblacion.size()/2);
        insertPoblacion(mejor_padre, poblacion);
    }
    

    epoca++;
}

    
/*
    USADO PARA LAS METAHEURÍSTICAS GENÉTICAS

    Hace como función de mostrar los resultados de la evolución genética de cualquiera de las
    metaheurísticas genéticas. Se coge la mejor solución de la población y la establece como
    solución al problema representado por la clase.
    
    RECOMENDABLE: Ejecutar antes varias épocas con 'runGenetico'. Si no se ha ejecutado ninguna
    se ejecutará por defecto una época de tipo estacionario y segmento uniforme.

    Nos devuelve True si se ha realizado con éxito el proceso de búsqueda de solución.
    Nos devuelve False en caso contrario.
*/
bool PAR::finishEpochs(){
    bool exito = true;
    
    // Si no hay población hacemos un inicio aleatorio de población de tamaño TAM_POBLACION
    if(poblacion.size()<TAM_POBLACION){
        exito = runEpoch(1,1,0,0,false);
    }

    // Añadimos el mejor fitness al recorrido
    recorrido_fun_objetivo.push_back(poblacion[0][poblacion[0].size()-1]);    

    if(!exito){
        cout << "Error PAR::endGenetico(). No se ha ejecutado la época bien.\n";
    }

    exito = simularSolucion(poblacion[0]);

    if(!exito){
        cout << "Error PAR::endGenetico(). Solución no válida.\n";
    }

    return exito;
}





/////////////////////////////////////////////////////////////
//
//    Métodos que nos dicen cómo de buena es la solución
//
/////////////////////////////////////////////////////////////

/*
    Calcula la distancia media intra-cluster de un cluster cluster
*/
double PAR::distanciaIntraCluster(int cluster){
    int size_c = clusters[cluster].size();
    double distancia = 0;

    if(necesidad_actualizar_centroides){
        necesidad_actualizar_centroides=false;
        updateCentroides();
    }
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
    double desviacion = 0;

    for(int c=0; c<num_clases; c++){
        desviacion += distanciaIntraCluster(c);
    }
    desviacion /= num_clases;

    return desviacion;
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
        -Función Objetivo
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