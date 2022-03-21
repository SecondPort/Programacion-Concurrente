// Tarea del 18/03

// Hacer un programa que busca un 1 en un arreglo de ceros
// = calloc(20000, sizeof(int));
// Versión 1 -> 1 hilo (secuencial) -> Ojo! Usar funciones!
// Versión 2 -> 2 hilos de búsqueda, cada hilo busca en la mitad
// Versión 3 -> (opcional) La búsqueda se reparte entre n hilos
//              con n = 2, 3, 4, ..., 9

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 20000

int *arreglo;
int posicion;

void *buscar(void *arg){
	int i;
	for(i=0; i<N; i++){
		if(arreglo[i]==1){
			posicion = i;
			break;
		}
	}
	pthread_exit(NULL);
}

int main(){
	int i;
	pthread_t hilo;
	arreglo = calloc(N, sizeof(int));//arreglo de ceros de tamaño N
	arreglo = (int *)malloc(N*sizeof(int));//reservando memoria para el arreglo de enteros

	srand(time(NULL));//generando un numero aleatorio para el arreglo
	for(i=0; i<N; i++){//llenando el arreglo con numeros aleatorios
		arreglo[i] = rand()%2;
	}

	arreglo = calloc(N, sizeof(int));//arreglo de ceros de tamaño N
	arreglo[rand()%N] = 1;//genera un 1 en una posición aleatoria del arreglo

	pthread_create(&hilo, NULL, buscar, NULL);
	pthread_join(hilo, NULL);

	printf("La posicion del 1 es: %d\n", posicion);
	free(arreglo);
	return 0;
}