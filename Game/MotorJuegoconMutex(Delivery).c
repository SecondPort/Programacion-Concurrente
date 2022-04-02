/* Se debe realizar el motor de un juego que denominaremos "Delivery". Este juego simulará la actividad de un local de comidas en el cual ingresan los pedidos por teléfono y los empleados tienen que preparar y entregar lo indicado.

En esta primer etapa se deberá trabajar sobre cuatro tipos de hilos:
encargado que da curso a los pedidos de los clientes y, luego de entregado el pedido, guarda el cobro en la caja,
telefono que genera los pedidos de los clientes,
cocinero que prepara la comida,
delivery que lleva los pedidos a los clientes,

El encargado atiende los pedidos que llegan por telefono separados un tiempo aleatorio. El cocinero prepara el pedido, demorando un tiempo diferente según la comida solicitada; solo puede preparar un pedido por vez. El delivery entrega los pedidos a los clientes (de a uno por vez), demorando un tiempo diferente (aleatorio) según la distancia, y al regresar entrega el monto del pedido al encargado.

Se debe sincronizar la interacción entre los hilos utilizando MUTEXES según la necesidad (revisar Split Mutex).
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

pedido_t pedido_generado;

int cantidad_pedidos = 0;

// Para evitar carreras, usamos mutexes.
pthread_mutex_t mutex_pedidos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_caja = PTHREAD_MUTEX_INITIALIZER;

// Los hilos deben esperar a que haya un pedido para empezar a hacer su trabajo, por lo que usamos semaforos
sem_t sem_pedidos;
sem_t sem_caja;
sem_t sem_cocineros;
sem_t sem_delivery;

// Funciones para los hilos.
void *generarPedido(void *arg)
{

    while (1)
    {
        sleep((rand() % 3) + 2); // wait 2 to 5 seconds
        cantidad_pedidos++;
        pedido_generado.numero = cantidad_pedidos;
        pedido_generado.tiempo_preparacion = (rand() % 10) + 2;
        pedido_generado.tiempo_delivery = (rand() % 10) + 2;

        printf("\nEl pedido numero %d con tiempo de preparacion %d y delivery %d ha sido generado", pedido_generado.numero, pedido_generado.tiempo_preparacion, pedido_generado.tiempo_delivery);

        pthread_mutex_lock(&mutex_pedidos);
        sem_post(&sem_pedidos);
        pthread_mutex_unlock(&mutex_pedidos);
    }

    return NULL;
}

void *cobrarPedido(void *arg)
{
    printf("\nInicio de hilo de cobro");
    while (1)
    {
        sem_wait(&sem_caja);
        printf("\nCobrando pedido numero %d", pedido_generado.numero);
        sleep((rand() % 5) + 2); // wait 2 to 5 seconds
        pthread_mutex_unlock(&mutex_caja);
    }

    return NULL;
}

void *prepararPedido(void *arg)
{

    printf("\nInicio de hilo de preparacion");

    while (1)
    {
        sem_wait(&sem_pedidos);

        printf("\nEl pedido numero %d ha ingresado a la cocina", pedido_generado.numero);
        sleep(pedido_generado.tiempo_preparacion);
        printf("\nEl pedido numero %d ha terminado de prepararse", pedido_generado.numero);

        sem_post(&sem_caja);
    }

    return NULL;
}

void *llevarPedido(void *arg)
{

    printf("\nInicio de hilo de delivery");

    while (1)
    {
        sem_wait(&sem_caja);

        printf("\nEl pedido numero %d ha ingresado al delivery", pedido_generado.numero);
        sleep(pedido_generado.tiempo_delivery);
        printf("\nEl pedido numero %d ha llegado a su destino", pedido_generado.numero);

        pthread_mutex_unlock(&mutex_pedidos);
    }

    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t hilo_telefono;
    pthread_t hilo_caja;
    pthread_t hilo_cocineros[3];
    pthread_t hilo_delivery[3];

    sem_init(&sem_pedidos, 0, 0);
    sem_init(&sem_caja, 0, 0);

    // Creamos hilos
    pthread_create(&hilo_telefono, NULL, generarPedido, NULL);
    pthread_create(&hilo_caja, NULL, cobrarPedido, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&hilo_cocineros[i], NULL, prepararPedido, NULL);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&hilo_delivery[i], NULL, llevarPedido, NULL);
    }

    // Esperamos a que finalicen la ejecucion de los hilos
    pthread_join(hilo_telefono, NULL);
    pthread_join(hilo_caja, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(hilo_cocineros[i], NULL);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(hilo_delivery[i], NULL);
    }

    return 0;
}