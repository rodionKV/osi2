#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int q = 0;

void *writer(void *arg) {

    for (int i = 1; i < 11;) {
        pthread_mutex_lock(&mutex);
        while (q != 1) pthread_cond_wait(&cond, &mutex);

        std::cout << "hello from second thread " << i++ << " times" << std::endl;

        q = 0;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);

    }

    return ((void *) nullptr);
}

int main() {
    int err;
    pthread_mutex_lock(&mutex);
    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create");
    }


    for (int i = 1; i < 11;) {
        pthread_mutex_unlock(&mutex);
        std::cout << "hello from main thread " << i++ << " times" << std::endl;
        q = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_lock(&mutex);
        while (q != 0) pthread_cond_wait(&cond, &mutex);


    }

    pthread_exit(nullptr);
}
