#ifndef _MONITORSIMPLE_H_
#define _MONITORSIMPLE_H_

typedef struct
{
    int elementos;//cantidad de elementos del buffer
    int datos[5];//buffer
    int lleno;//indica si el buffer esta lleno
    int inicio,fin;//indices del buffer
    int to_read;//indica si hay datos para leer
    pthread_cond_t cond1,cond2;//señales para indicar que se puede escribir y leer
    pthread_condattr_t condattr1,condattr2;//atributos de las señales
    pthread_mutex_t mutex;//mutex para bloquear el acceso a los datos
    pthread_mutexattr_t mutexattr;//atributos del mutex

}Monitor;

void GuardarDato(Monitor *monitor,int dato);
void LeerDato(Monitor *monitor,int *dato);

#endif
