//
// Created by rodion on 16.10.22.
//
#include <iostream>
#include <pthread.h>
#include <semaphore>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
pthread_t ntid;
sem_t *z,*t;

int main() {
    int err;
    t = sem_open("/hello11",O_CREAT,  S_IRWXU,1);
    z = sem_open("/hello22",O_CREAT,  S_IRWXU,0);

    if (fork()==0){
        for (int j = 1; j < 11; ++j) {
            sem_wait(z);
            std::cout << "hello from second thread " << j << " times" << std::endl;
            sem_post(t);
        }
    } else {

        for (int i = 1; i < 11; ++i) {
            sem_wait(t);
            std::cout << "hello from main thread " << i << " times" << std::endl;
            sem_post(z);
        }

        sem_close(z);
        sem_close(t);
    }
    return 0;
}