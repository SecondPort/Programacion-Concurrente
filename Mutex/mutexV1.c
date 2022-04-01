#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS 4

int *array;
int max;
int max_pos;
int size;
pthread_mutex_t mutex;

void *max_search(void *arg)
{
    int id = *((int *)arg);
    int start = id * (size / MAX_THREADS);
    int end = (id + 1) * (size / MAX_THREADS);
    int local_max = 0;
    int local_max_pos = 0;
    for (int i = start; i < end; i++)
    {
        if (array[i] > local_max)
        {
            local_max = array[i];
            local_max_pos = i;
        }
    }
    pthread_mutex_lock(&mutex);
    if (local_max > max)
    {
        max = local_max;
        max_pos = local_max_pos;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    size = atoi(argv[1]);
    array = (int *)malloc(sizeof(int) * size);
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % size;
    }
    max = array[0];
    max_pos = 0;
    pthread_t threads[MAX_THREADS];
    int thread_args[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < MAX_THREADS; i++)
    {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, max_search, (void *)&thread_args[i]);
    }
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("Max: %d\n", max);
    printf("Max pos: %d\n", max_pos);
    pthread_mutex_destroy(&mutex);
    free(array);
    return 0;
}