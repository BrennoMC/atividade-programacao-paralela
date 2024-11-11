
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;
int shared_data = 0;

void* reader(void* arg) {
    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) {
        pthread_mutex_lock(&write_mutex); // Primeiro leitor bloqueia escritores
    }
    pthread_mutex_unlock(&mutex);

    printf("Reader %ld read data: %d\n", (long)arg, shared_data);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) {
        pthread_mutex_unlock(&write_mutex); // Último leitor libera escritores
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* writer(void* arg) {
    pthread_mutex_lock(&write_mutex);
    shared_data++;
    printf("Writer %ld updated data to: %d\n", (long)arg, shared_data);
    pthread_mutex_unlock(&write_mutex);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[3];

    for (long i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)i);
    }
    for (long i = 0; i < 3; i++) {
        pthread_create(&writers[i], NULL, writer, (void*)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);

    return 0;
}
