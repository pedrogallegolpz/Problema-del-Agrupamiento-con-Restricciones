/* GENERADOR DE NUMEROS ALEATORIOS */

#ifndef __RANDOM_PPIO_H
#define __RANDOM_PPIO_H

#include <vector>

using namespace std;

/* Inicializa la semilla al valor 'x'.
   Solo debe llamarse a esta funcion una vez en todo el programa */
void Set_random (unsigned long x);

/* Devuelve el valor actual de la semilla */
unsigned long Get_random (void);

/* Genera un numero aleatorio real en el intervalo [0,1[
   (incluyendo el 0 pero sin incluir el 1) */
double Rand(void);

/* Genera un numero aleatorio entero en {low,...,high} */
int Randint(int low, int high);

/* Genera un numero aleatorio real en el intervalo [low,...,high[
   (incluyendo 'low' pero sin incluir 'high') */
double Randfloat(double low, double high);

/*
Barajar un vector*/
vector<int> ShuffleIndices(int size);


#endif
