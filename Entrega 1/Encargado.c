#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "Monitor.h"

typedef struct
{
    //sincronizar caja cliente
    sem_t *semCajaRecibe[3];//semaforo para que en cada caja se le indique que tiene que atender
    sem_t *semCahaEntrega;
    sem_t *semClientesEnEspera;//maximo 6 cleintes en espera, si no se retira del local
    int numCaja;//indicar el numero de cajas
    sem_t *
};
