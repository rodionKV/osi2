#include <iostream>
#include <pthread.h>
#include <semaphore>
#include <semaphore.h>
#include <unistd.h>
pthread_t ntid;
sem_t z,t;

void* writer(void *arg) {
    for (int i = 1; i < 11; ++i) {
        sem_wait(&z);
        std::cout << "hello from second thread " << i << " times" << std::endl;
        sem_post(&t);
    }
    return ((void *)nullptr);
}

int main() {
    int err;

    sem_init(&z,0,1);
    sem_init(&t,0,1);
    sem_wait(&z);
    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create");
    }

    for (int i = 1; i < 11; ++i) {
        sem_wait(&t);
        std::cout << "hello from main thread " << i << " times" << std::endl;
        sem_post(&z);
    }

    sem_destroy(&z);
    sem_destroy(&t);

    pthread_exit(nullptr);
}