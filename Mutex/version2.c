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

int *A;
int encontrado = 0;

void *buscar(void *arg) {
    int i;
    int *id = (int *) arg;//id del hilo
    int inicio = (*id) * (N / 2);//inicio de la mitad del arreglo
    int fin = inicio + (N / 2);//fin de la mitad del arreglo
    for (i = inicio; i < fin; i++) {//busca en la mitad del arreglo
        if (A[i] == 1) {
            encontrado = 1;
            printf("Encontrado en %d\n", i);
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    int i;//contador
    pthread_t hilo1, hilo2;//hilos

    int id1 = 0, id2 = 1;//id de los hilos (0, 1) para buscar en la mitad del arreglo
    A = calloc(N, sizeof(int));//arreglo de ceros de tamaño N
    srand(time(NULL));//semilla para generar números aleatorios
    A[rand() % N] = 1;//genera un 1 en una posición aleatoria del arreglo

    pthread_create(&hilo1, NULL, buscar, (void *) &id1);//crea el hilo 1
    pthread_create(&hilo2, NULL, buscar, (void *) &id2);//crea el hilo 2

    pthread_join(hilo1, NULL);//espera a que el hilo 1 termine
    pthread_join(hilo2, NULL);//espera a que el hilo 2 termine

    if (!encontrado) {//si no encontró el 1 en el arreglo
        printf("No encontrado\n");//imprime que no encontró el 1
    }
    free(A);//libera el arreglo
    return 0;
}

