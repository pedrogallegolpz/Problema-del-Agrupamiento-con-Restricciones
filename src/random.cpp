#include <math.h>
#include <vector>
#include "../include/random.h"


using namespace std;

unsigned long Seed = 0L;

#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

void Set_random (unsigned long x)
/* Inicializa la semilla al valor x.
   Solo debe llamarse a esta funcion una vez en todo el programa */
{
    Seed = (unsigned long) x;

}

unsigned long Get_random (void)
/* Devuelve el valor actual de la semilla */
{
    return Seed;
}

double Rand(void)
/* Genera un numero aleatorio real en el intervalo [0,1[
   (incluyendo el 0 pero sin incluir el 1) */
{
    return (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE );
}

int Randint(int low, int high)
/* Genera un numero aleatorio entero en {low,...,high} */
{
    return (int) (low + (high-(low)+1) * Rand());
}

double Randfloat(double low, double high)
/* Genera un numero aleatorio real en el intervalo [low,...,high[
   (incluyendo 'low' pero sin incluir 'high') */
{
    return (low + (high-(low))*Rand());
}


vector<int> ShuffleIndices(int size)
/*
Barajar un vector*/
{
    vector<int> indices;
    for(int i=0; i<size; i++){
        indices.push_back(i);
    }

    vector<int> indices_shuffled;
    int i=0;
    while(i<indices.size()){
        int j=Randint(0,indices.size());

        // Insertamos el índice
        indices_shuffled.push_back(indices[j]);

        // Lo borramos del primer vector
        indices.erase(indices.begin()+j);
    }

    return indices_shuffled;
}
