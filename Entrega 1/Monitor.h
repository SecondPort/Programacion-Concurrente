#ifndef _MONITORSIMPLE_H_
#define _MONITORSIMPLE_H_

typedef struct
{
    int elementos;
    int datos[5];
    int lleno;
    int inicio,fin;
    int to_read;
    pthread_cond_t cond1,cond2;
    pthread_condattr_t condattr1,condattr2;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;

}Monitor;

void GuardarDato(Monitor *monitor,int dato);
void LeerDato(Monitor *monitor,int *dato);

#endif
