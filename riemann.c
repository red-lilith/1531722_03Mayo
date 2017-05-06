/**
* FILE: riemann_erroneo.c
* DESCRIPTION:
* 	Implementacion de la suma de Riemann para calcular el area bajo la
*	curva.
* AUTHOR: John Sanabria - john.sanabria@correounivalle.edu.co
* LAST REVISED: 14/04/16
* REFERENCES: http://mathinsight.org/calculating_area_under_curve_riemann_sums
******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define MAXRECT 1000000
#define MAXTHREAD 10
#define BASE 0.0
#define LIMIT 1.0

double length = LIMIT - BASE; // La longitud en el eje 'x'
double numRectxThread = (double)(MAXRECT/MAXTHREAD); // Cuantos hilos/rectangulo
double base_length = 0; // Variable que tiene el ancho de cada rectangulo
double sumTotal = 0; // Variable que tendra la suma total
double partialSums[MAXTHREAD]; // Variable que tien la suma parcial de c/hilo

// Funcion a la que se le calculara la integral, x^2
double function(double x) {
	return x * x; 
}
void* calcular(void *arg) {
	int id = (int)arg;
	double loslimit = id*(length/MAXTHREAD);
	partialSums[id]=0;
	for(int i=loslimit;i<(loslimit+numRectxThread);i++){
		double area = function(loslimit) * base_length;
		partialSums[id] += area;
		loslimit += base_length;
	}
	printf("sum: %lf\n", partialSums[id]);
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	pthread_t thread_id[MAXTHREAD];	
	int taskids[MAXTHREAD];

	base_length = length/MAXRECT; // esta variable contiene el ancho de
				      // cada base de cada rectangulo
	printf("base length: %lf numRectxThread: %lf\n",base_length, numRectxThread);
	// Creacion de los hilos que calcularan el area bajo la curva
	for (int i = 0; i < MAXTHREAD; i++) {
		taskids[i] = i;
		pthread_create(&thread_id[i],NULL,calcular,(void*)taskids[i]);
	}
	// Ciclo donde se espera el resultado de cada hilo
	for (int i = 0; i < MAXTHREAD; i++) {
		pthread_join(thread_id[i],NULL);
		sumTotal += partialSums[i];
	}
	printf("Suma total %lf\n",sumTotal);
	pthread_exit(NULL);
}
