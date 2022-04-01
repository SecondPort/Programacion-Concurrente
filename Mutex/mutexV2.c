
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 9

int P, n, *A, max, max_pos;
pthread_mutex_t mutex;

void *buscar(void *arg) {
    int id = *(int *)arg;
    int i, ini, fin;
    ini = id * (P / n);
    fin = (id + 1) * (P / n);
    if (id == n - 1)
        fin = P;
    for (i = ini; i < fin; i++) {
        if (A[i] > max) {
            pthread_mutex_lock(&mutex);
            max = A[i];
            max_pos = i;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i, j
    ;
    pthread_t tid[MAX_THREADS];
    int thread_id[MAX_THREADS];
    srand(time(NULL));
    printf("Ingrese el tamaño del arreglo: ");
    scanf("%d", &P);
    printf("Ingrese el número de hilos: ");
    scanf("%d", &n);
    A = (int *)malloc(P * sizeof(int));
    for (i = 0; i < P; i++)
        A[i] = rand() % P;
    j = rand() % P;
    A[j] = P;
    printf("El valor máximo %d está en la posición %d\n", P, j);
    max = 0;
    max_pos = 0;
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < n; i++) {
        thread_id[i] = i;
        pthread_create(&tid[i], NULL, buscar, (void *)&thread_id[i]);
    }
    for (i = 0; i < n; i++)
        pthread_join(tid[i], NULL);
    printf("El valor máximo %d está en la posición %d\n", max, max_pos);
    pthread_mutex_destroy(&mutex);
    free(A);
    return 0;
}