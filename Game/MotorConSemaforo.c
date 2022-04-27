/*Se debe realizar el motor de un juego que denominaremos "Delivery". Este juego simulará la actividad de un local de comidas en el cual ingresan los pedidos por teléfono y los empleados tienen que preparar y entregar lo indicado.

En esta primer etapa se deberá trabajar sobre cuatro tipos de hilos:
encargado que da curso a los pedidos de los clientes y, luego de entregado el pedido, guarda el cobro en la caja,
telefono que genera los pedidos de los clientes,
cocinero que prepara la comida,
delivery que lleva los pedidos a los clientes,

El encargado atiende los pedidos que llegan por telefono separados un tiempo aleatorio. El cocinero prepara el pedido, demorando un tiempo diferente según la comida solicitada; solo puede preparar un pedido por vez. El delivery entrega los pedidos a los clientes (de a uno por vez), demorando un tiempo diferente (aleatorio) según la distancia, y al regresar entrega el monto del pedido al encargado.

Esta versión tendrá 3 cocineros y 2 delivery.

Se debe sincronizar la interacción entre los hilos utilizando SEMÁFOROS según la necesidad (revisar Split Binary Semaphore)

*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

typedef struct pedido
{
    int numero;
    int tiempo_preparacion;
    int tiempo_delivery;
} pedido_t;

typedef struct cocinero
{
    int numero;
    int tiempo_preparacion;
    int tiempo_delivery;
} cocinero_t;

typedef struct delivery
{
    int numero;
    int tiempo_preparacion;
    int tiempo_delivery;
} delivery_t;

typedef struct encargado
{
    int numero;
    int tiempo_preparacion;
    int tiempo_delivery;
} encargado_t;

typedef struct telefono
{
    int numero;
    int tiempo_preparacion;
    int tiempo_delivery;
} telefono_t;

sem_t sem_cocinero;
sem_t sem_delivery;
sem_t sem_encargado;
sem_t sem_telefono;

void *cocinero(void *arg)
{
    cocinero_t *cocinero = (cocinero_t *)arg;
    while (1)
    {
        sem_wait(&sem_cocinero);
        printf("Cocinero %d preparando pedido\n", cocinero->numero);
        sleep(cocinero->tiempo_preparacion);
        printf("Cocinero %d termino de preparar pedido\n", cocinero->numero);
        sem_post(&sem_delivery);
    }
}

void *delivery(void *arg)
{
    delivery_t *delivery = (delivery_t *)arg;
    while (1)
    {
        sem_wait(&sem_delivery);
        printf("Delivery %d entregando pedido\n", delivery->numero);
        sleep(delivery->tiempo_delivery);
        printf("Delivery %d termino de entregar pedido\n", delivery->numero);
        sem_post(&sem_encargado);
    }
}

void *encargado(void *arg)
{
    encargado_t *encargado = (encargado_t *)arg;
    while (1)
    {
        sem_wait(&sem_encargado);
        printf("Encargado %d cobrando pedido\n", encargado->numero);
        sleep(encargado->tiempo_preparacion);
        printf("Encargado %d termino de cobrar pedido\n", encargado->numero);
        sem_post(&sem_telefono);
    }
}

void *telefono(void *arg)
{
    telefono_t *telefono = (telefono_t *)arg;
    while (1)
    {
        sem_wait(&sem_telefono);
        printf("Telefono %d recibiendo pedido\n", telefono->numero);
        sleep(telefono->tiempo_delivery);
        printf("Telefono %d termino de recibir pedido\n", telefono->numero);
        sem_post(&sem_cocinero);
    }
}

int main(int argc, char *argv[])
{
    pthread_t cocinero1, cocinero2, cocinero3, delivery1, delivery2, encargado1, telefono1;

    cocinero_t cocinero_1 = {1, 1, 1};
    cocinero_t cocinero_2 = {2, 2, 2};
    cocinero_t cocinero_3 = {3, 3, 3};

    delivery_t delivery_1 = {1, 1, 1};
    delivery_t delivery_2 = {2, 2, 2};

    encargado_t encargado_1 = {1, 1, 1};

    telefono_t telefono_1 = {1, 1, 1};

    sem_init(&sem_cocinero, 0, 0);
    sem_init(&sem_delivery, 0, 0);
    sem_init(&sem_encargado, 0, 0);
    sem_init(&sem_telefono, 0, 1);

    pthread_create(&cocinero1, NULL, cocinero, &cocinero_1);
    pthread_create(&cocinero2, NULL, cocinero, &cocinero_2);
    pthread_create(&cocinero3, NULL, cocinero, &cocinero_3);
    pthread_create(&delivery1, NULL, delivery, &delivery_1);
    pthread_create(&delivery2, NULL, delivery, &delivery_2);
    pthread_create(&encargado1, NULL, encargado, &encargado_1);
    pthread_create(&telefono1, NULL, telefono, &telefono_1);

    pthread_join(cocinero1, NULL);
    pthread_join(cocinero2, NULL);
    pthread_join(cocinero3, NULL);
    pthread_join(delivery1, NULL);
    pthread_join(delivery2, NULL);
    pthread_join(encargado1, NULL);
    pthread_join(telefono1, NULL);

    sem_destroy(&sem_cocinero);
    sem_destroy(&sem_delivery);
    sem_destroy(&sem_encargado);
    sem_destroy(&sem_telefono);

    return 0;
}