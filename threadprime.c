/**
---- threadprime.c ----
1- Programa que muestra como contar la cantidad de numeros primos que se 
   encuentra en un rango de numeros, desde LOW hasta HIGH.
El metodo usado para determinar si un numero N es primo es buscar un
numero en el rango 2..N/2 que divida exactamente a N. De existir dicho numero 
entonces N no es primo, de otra forma N es primo.
Autor: John Sanabria
Fecha: Mayo 3, 2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define LOW 5000000
#define HIGH 5100000
#define MAXTHREAD 130

long countarray[MAXTHREAD];
int chunksize;

// Funcion usada para determinar si un numero 'p' es primo o no
// Esta funcion retorna '1' si 'p' es primo, de otra forma '0'
int isprime(int p) {
	int i,tope;

	// Si es un numero entre 2 y 5, entonces es primo
	if (p == 2 || p == 3)
		return 1;
	// Determinamos cual es la mitad de 'p'
	tope = (int)(p/2);
	// Recorremos los numeros desde 2 hasta p/2
	for (i = 2; i <= tope; i++) 
		if (p%i == 0) // si 'p' es divido exactamente por i, no es primo
			break;
	// si 'i' toma el valor 'tope+1' entonces quiere decir que no se 
	// encontro un numero que dividiera exactamente a 'p' luego 'p' es primo
	if (i == tope + 1) 
		return 1;
	return 0;
}

// Funcion con el prototipo REQUERIDO por 'pthread' para asignar esta funcion
// a un hilo de ejecucion
void* worktodo(void* arg) {
	int id = (int)arg;
	int i;
	countarray[id] = 0;
	for (i = id*chunksize+LOW; i < (id+1)*chunksize+LOW; i++) {
		if (isprime(i))
			countarray[id]++;
	}
	return NULL;
}

int main(int argc, char** argv) {
	long count; // almacena el total de primos en el rango [ HIGH - LOW ]
	int i, 
	    numworkers; // numero de hilos para esta ejecucion
	pthread_t ptarray[MAXTHREAD]; // almacena 'id' de hilos
	count = 0;
	
	// 
	// Bloque de inicializacion
	//
	if (argc != 2) {
		printf("Por favor digite un numero hilos a usar\n");
		printf("\t%s <numhilos>\n",argv[0]);
		exit(-1);
	}
	numworkers = atoi(argv[1]);
	if (numworkers > MAXTHREAD || numworkers == 0) {
		printf("Maximo numero de hilos es %d, usted solicito %d\n",MAXTHREAD, numworkers);
		exit(-1);
	}
	// Se determina el numero de numeros a procesar por cada hilo
	chunksize = (int)((HIGH - LOW)/numworkers);
	// Se crea y se le asigna a cada hilo un identificador e.g. 0, 1,...
	for (i = 0; i < numworkers; i++) {
		countarray[i] = (long)i;
		pthread_create(&ptarray[i],NULL, worktodo, (void*)countarray[i]);
	}

	// Se espera por la terminacion de ejecucion de cada hilo
	for (i = 0; i < numworkers; i++) {
		pthread_join(ptarray[i],NULL);
		count += countarray[i]; // Se suma lo que conto cada hilo
		printf("Hilo %d: %ld primos\n",(i+1), countarray[i]);
	}
	printf("Total primos %ld\n", count);
}
