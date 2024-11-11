#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

// Estrutura da barreira
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int num_threads;
} my_barrier_t;

// Função para inicializar a barreira
void my_barrier_init(my_barrier_t* barrier, int num_threads) {
    barrier->num_threads = num_threads;
    barrier->count = 0;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

// Função para a barreira
void my_barrier_wait(my_barrier_t* barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;

    if (barrier->count == barrier->num_threads) {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);  // Libera todas as threads
    } else {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);  // Aguarda as outras threads
    }

    pthread_mutex_unlock(&barrier->mutex);
}

// Função para destruir a barreira
void my_barrier_destroy(my_barrier_t* barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}

// Função executada por cada thread
void* task(void* arg) {
    printf("Thread %ld chegou na barreira\n", (long)arg);
    my_barrier_wait((my_barrier_t*)arg);  // Aguardar na barreira
    printf("Thread %ld passou pela barreira\n", (long)arg);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    my_barrier_t barrier;

    // Inicializar a barreira
    my_barrier_init(&barrier, NUM_THREADS);

    // Criar as threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, task, (void*)&barrier);
    }

    // Esperar pela conclusão das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruir a barreira
    my_barrier_destroy(&barrier);

    return 0;
}
