#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N          5
#define COMIDAS    3

sem_t tenedor[N];
sem_t mesa;

void *filosofo(void *arg)
{
    int id = *(int *)arg;
    int izq = id;
    int der = (id + 1) % N;

    for (int i = 1; i <= COMIDAS; i++) {

        printf("Filosofo %d esta pensando\n", id);
        usleep(1000 + rand() % 2000);

        printf("Filosofo %d tiene hambre\n", id);
        sem_wait(&mesa);

        sem_wait(&tenedor[izq]);
        printf("Filosofo %d tomo el tenedor %d\n", id, izq);
        sem_wait(&tenedor[der]);
        printf("Filosofo %d tomo el tenedor %d\n", id, der);

        printf("Filosofo %d esta comiendo (comida %d de %d)\n", id, i, COMIDAS);
        usleep(1000 + rand() % 2000);

        sem_post(&tenedor[der]);
        sem_post(&tenedor[izq]);
        printf("Filosofo %d solto los tenedores %d y %d\n", id, izq, der);

        sem_post(&mesa);
    }

    printf("Filosofo %d termino de cenar\n", id);
    return NULL;
}

int main(void)
{
    pthread_t hilos[N];
    int ids[N];

    for (int i = 0; i < N; i++)
        sem_init(&tenedor[i], 0, 1);
    sem_init(&mesa, 0, N - 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, filosofo, &ids[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(hilos[i], NULL);

    for (int i = 0; i < N; i++)
        sem_destroy(&tenedor[i]);
    sem_destroy(&mesa);

    printf("Todos los filosofos terminaron de cenar\n");
    return 0;
}