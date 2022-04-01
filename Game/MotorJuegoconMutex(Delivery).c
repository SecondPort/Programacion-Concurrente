/* Se debe realizar el motor de un juego que denominaremos "Delivery". Este juego simulará la actividad de un local de comidas en el cual ingresan los pedidos por teléfono y los empleados tienen que preparar y entregar lo indicado.

En esta primer etapa se deberá trabajar sobre cuatro tipos de hilos:
encargado que da curso a los pedidos de los clientes y, luego de entregado el pedido, guarda el cobro en la caja,
telefono que genera los pedidos de los clientes,
cocinero que prepara la comida,
delivery que lleva los pedidos a los clientes,

El encargado atiende los pedidos que llegan por telefono separados un tiempo aleatorio. El cocinero prepara el pedido, demorando un tiempo diferente según la comida solicitada; solo puede preparar un pedido por vez. El delivery entrega los pedidos a los clientes (de a uno por vez), demorando un tiempo diferente (aleatorio) según la distancia, y al regresar entrega el monto del pedido al encargado.

Se debe sincronizar la interacción entre los hilos utilizando MUTEXES según la necesidad (revisar Split Mutex). */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define N_CLIENTES 10
#define N_EMPLEADOS 3
#define N_PEDIDOS 10
#define N_COCINEROS 3
#define N_DELIVERY 3
#define N_CAJAS 3
#define N_PEDIDOS_DELIVERY 3
#define N_PEDIDOS_COCINA 3
#define N_PEDIDOS_TELEFONO 3
#define N_PEDIDOS_CAJA 3
#define N_PEDIDOS_ENCARGADO 3

#define TIEMPO_PEDIDO_MIN 5
#define TIEMPO_PEDIDO_MAX 10
#define TIEMPO_COCINA_MIN 5
#define TIEMPO_COCINA_MAX 10
#define TIEMPO_DELIVERY_MIN 5
#define TIEMPO_DELIVERY_MAX 10
#define TIEMPO_CAJA_MIN 5
#define TIEMPO_CAJA_MAX 10
#define TIEMPO_ENCARGADO_MIN 5
#define TIEMPO_ENCARGADO_MAX 10

#define TIEMPO_PEDIDO_MIN_COCINA 5
#define TIEMPO_PEDIDO_MAX_COCINA 10
#define TIEMPO_PEDIDO_MIN_DELIVERY 5
#define TIEMPO_PEDIDO_MAX_DELIVERY 10
#define TIEMPO_PEDIDO_MIN_CAJA 5
#define TIEMPO_PEDIDO_MAX_CAJA 10
#define TIEMPO_PEDIDO_MIN_ENCARGADO 5
#define TIEMPO_PEDIDO_MAX_ENCARGADO 10

#define TIEMPO_PEDIDO_MIN_TELEFONO 5
#define TIEMPO


void *hilo_pedido_telefono(void *arg);
void *hilo_pedido_cocina(void *arg);
void *hilo_pedido_delivery(void *arg);
void *hilo_pedido_caja(void *arg);
void *hilo_pedido_encargado(void *arg);

void *hilo_cocina(void *arg);
void *hilo_delivery(void *arg);
void *hilo_caja(void *arg);
void *hilo_encargado(void *arg);

void *hilo_pedido_telefono_cocina(void *arg);
void *hilo_pedido_telefono_delivery(void *arg);
void *hilo_pedido_telefono_caja(void *arg);
void *hilo_pedido_telefono_encargado(void *arg);


int main(int argc, char *argv[]) {
    pthread_t hilos[N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY + N_PEDIDOS_COCINA + N_PEDIDOS_TELEFONO + N_PEDIDOS_CAJA + N_PEDIDOS_ENCARGADO];
    int i;
    int *id;

    srand(time(NULL));

    for (i = 0; i < N_CLIENTES; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i], NULL, hilo_pedido_telefono, (void *) id);
    }

    for (i = 0; i < N_EMPLEADOS; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES], NULL, hilo_pedido_cocina, (void *) id);
    }

    for (i = 0; i < N_COCINEROS; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS], NULL, hilo_cocina, (void *) id);
    }

    for (i = 0; i < N_DELIVERY; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS], NULL, hilo_delivery, (void *) id);
    }

    for (i = 0; i < N_CAJAS; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY], NULL, hilo_caja, (void *) id);
    }

    for (i = 0; i < N_PEDIDOS_DELIVERY; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS], NULL, hilo_pedido_delivery, (void *) id);
    }

    for (i = 0; i < N_PEDIDOS_COCINA; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY], NULL, hilo_pedido_cocina, (void *) id);
    }

    for (i = 0; i < N_PEDIDOS_TELEFONO; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY + N_PEDIDOS_COCINA], NULL, hilo_pedido_telefono, (void *) id);
    }

    for (i = 0; i < N_PEDIDOS_CAJA; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY + N_PEDIDOS_COCINA + N_PEDIDOS_TELEFONO], NULL, hilo_pedido_caja, (void *) id);
    }

    for (i = 0; i < N_PEDIDOS_ENCARGADO; i++) {
        id = malloc(sizeof(int));
        *id = i;
        pthread_create(&hilos[i + N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY + N_PEDIDOS_COCINA + N_PEDIDOS_TELEFONO + N_PEDIDOS_CAJA], NULL, hilo_pedido_encargado, (void *) id);
    }

    for (i = 0; i < N_CLIENTES + N_EMPLEADOS + N_COCINEROS + N_DELIVERY + N_CAJAS + N_PEDIDOS_DELIVERY + N_PEDIDOS_COCINA + N_PEDIDOS_TELEFONO + N_PEDIDOS_CAJA + N_PEDIDOS_ENCARGADO; i++) {
        pthread_join(hilos[i], NULL);
    }

    return 0;
}


void *hilo_pedido_telefono(void *arg) {
    int id = *((int *) arg);
    int i;
    int cantidad;
    int producto;
    int cliente;
    int empleado;
    int cocina;
    int delivery;
    int caja;
    int encargado;
    int pedido;
    int telefono;
    int pedido_cocina;
    int pedido_delivery;
    int pedido_caja;
    int pedido_encargado;
    int pedido_telefono;
    int pedido_cocina_delivery;
    int pedido_cocina_caja;
    int pedido_cocina_encargado;
    int pedido_cocina_telefono;
    int pedido_delivery_caja;
    int pedido_delivery_encargado;
    int pedido_delivery_telefono;
    int pedido_caja_encargado;
    int pedido_caja_telefono;
    int pedido_encargado_telefono;
    int pedido_cocina_delivery_caja;
    int pedido_cocina_delivery_encargado;
    int pedido_cocina_delivery_telefono;
    int pedido_cocina_caja_encargado;
    int pedido_cocina_caja_telefono;
    int pedido_cocina_encargado_telefono;
    int 