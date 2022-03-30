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

    printf("Thread %d: start=%d, end=%d\n", id, start, end);

    for (i = start; i < end; i++)
    {
        if (array[i] == 1)
        {
            found = 1;
            printf("Thread %d: found at %d\n", id, i);
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    int id[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    printf("Enter array size: ");
    scanf("%d", &size);

    array = (int *)malloc(sizeof(int) * size);

    srand(time(NULL));
    int random = rand() % size;
    array[random] = 1;

    for (i = 0; i < MAX_THREADS; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, search, (void *)&id[i]);
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

    return 0;
}