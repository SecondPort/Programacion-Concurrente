#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 9

int *array;
int size;
int found = 0;

void *search(void *arg)
{
    int id = *((int *)arg);
    int start = (size / MAX_THREADS) * id;
    int end = start + (size / MAX_THREADS);
    int i;

    for (i = start; i < end; i++)
    {
        if (array[i] == 1)
        {
            found = 1;
            printf("Thread %d found 1 at %d\n", id, i);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    int *ids;
    pthread_t threads[MAX_THREADS];

    if (argc < 2)
    {
        printf("Usage: %s <size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    size = atoi(argv[1]);
    array = (int *)malloc(sizeof(int) * size);

    srand(time(NULL));
    array[rand() % size] = 1;

    ids = (int *)malloc(sizeof(int) * MAX_THREADS);
    for (i = 0; i < MAX_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, search, (void *)&ids[i]);
    }

    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    if (!found)
    {
        printf("Not found\n");
    }

    free(array);
    free(ids);

    return 0;
}