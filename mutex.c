/*Realizar un programa que busca el valor máximo de un arreglo de datos. El tamaño del arreglo lo determina el usuario ingresándolo por teclado.

El arreglo contendrá valores aleatorios entre 0 y P-1 y el valor máximo P deberá colocarse en una posición aleatoria del arreglo.

La búsqueda debe realizarse mediante una función que reciba por parámetro las posiciones inicial y final. El valor máximo estará contenido en una variable común (compartida) entre todos los hilos.

El programa deberá entregarse en las siguientes versiones:

Proceso secuencial,

La versión para n hilos debe distribuir equitativamente la tarea de búsqueda entre todos los hilos. El acceso a la variable donde se almacena la posición del valor máximo debe controlarse mediante la utilización de un MUTEX.*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 4

int *array;
int max;
int max_pos;
int size;
pthread_mutex_t mutex;

void *search(void *arg)
{
    int id = *((int *)arg);
    int i;
    int local_max = 0;
    int local_max_pos = 0;
    int start = id * (size / MAX_THREADS);
    int end = (id + 1) * (size / MAX_THREADS);
    if (id == MAX_THREADS - 1)
        end = size;
    for (i = start; i < end; i++)
    {
        if (array[i] > local_max)
        {
            local_max = array[i];
            local_max_pos = i;
        }
    }
    pthread_mutex_lock(&mutex);
    if (
        local_max > max)
    {
        max = local_max;
        max_pos = local_max_pos;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    int *id;
    pthread_t threads[MAX_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutex, NULL);
    printf("Ingrese el tamaño del arreglo: ");
    scanf("%d", &size);
    array = (int *)malloc(sizeof(int) * size);
    srand(time(NULL));
    max = rand() % size;
    max_pos = rand() % size;
    array[max_pos] = max;
    for (i = 0; i < size; i++)
    {
        if (i != max_pos)
            array[i] = rand() % size;
    }
    for (i = 0; i < MAX_THREADS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], &attr, search, (void *)id);
    }
    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("El valor máximo es %d y está en la posición %d\n", max, max_pos);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}