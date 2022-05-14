#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "Monitor.h"

void GuardarDato(Monitor *monitor,int dato)
{
    pthread_mutex_lock(&monitor->mutex);//el mutex se bloquea para que no se pueda escribir hasta que se desbloquea
    while(monitor->lleno == 1)//si el buffer esta lleno, se espera a que se desbloquee
    {
        pthread_cond_wait(&monitor->cond1,&monitor->mutex);//se espera a que se desbloquee el mutex
    }
    monitor->datos[monitor->fin] = dato;//se guarda el dato en el buffer
    monitor->to_read = 1;//se indica que hay al menos un dato para leer
    monitor->fin = ++monitor->fin % (monitor->elementos);//se actualiza el indice del buffer
    if(monitor->fin == monitor->inicio)//si el indice del buffer llega al final, se indica que el buffer esta lleno
    {
        monitor->lleno = 1;
    }
    pthread_cond_signal(&monitor->cond2);//se indica que se puede leer
    pthread_mutex_unlock(&monitor->mutex);//se desbloquea el mutex
}

void LeerDato (Monitor *monitor,int *dato)
{
    pthread_mutex_lock(&monitor->mutex);//el mutex se bloquea para que no se pueda escribir hasta que se desbloquea
    while(monitor->to_read == 0)//si no hay datos para leer, se espera a que se escriban
    {
        pthread_cond_wait(&monitor->cond2,&monitor->mutex);//se espera a que se escriban datos
    }
    *dato = monitor->datos[monitor->inicio];//se lee el dato del buffer
    monitor->inicio = ++monitor->inicio % (monitor->elementos);//se actualiza el indice del buffer
    monitor->lleno = 0;//se indica que el buffer esta vacio
    if (monitor->inicio == monitor->fin)//si el indice del buffer llega al final, se indica que el buffer esta vacio
    {
        monitor->to_read = 0;//si llega al final, no hay datos para leer
    }
    pthread_cond_signal(&monitor->cond1);//se indica que se puede escribir
    pthread_mutex_unlock(&monitor->mutex);//se desbloquea el mutex
}
