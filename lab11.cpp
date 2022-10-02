#include <iostream>
#include <pthread.h>

pthread_t ntid;
pthread_mutex_t arr_mutex[3];

void* writer(void *arg) {
    pthread_mutex_lock(&arr_mutex[1]);
    for (int i = 0; i < 21; ++i) {
        pthread_mutex_lock(&arr_mutex[0]);
        std::cout << "hello from second thread " << i << " times" << std::endl;
        pthread_mutex_unlock(&arr_mutex[1]);
        pthread_mutex_lock(&arr_mutex[2]);
        pthread_mutex_unlock(&arr_mutex[0]);
        pthread_mutex_lock(&arr_mutex[1]);
        pthread_mutex_unlock(&arr_mutex[2]);

    }
    pthread_mutex_unlock(&arr_mutex[1]);
    pthread_mutex_destroy(&arr_mutex[1]);
    return ((void *)nullptr);
}

int main() {
    pthread_mutexattr_t pthreadMutexattr;
    pthread_mutexattr_init(&pthreadMutexattr);
    pthread_mutexattr_settype(&pthreadMutexattr,PTHREAD_MUTEX_ERRORCHECK);

    int err;
    for (auto & i : arr_mutex) {
        err = pthread_mutex_init(&i, &pthreadMutexattr);

        if (err != 0) {
            perror("error create mutex");
        }
    }

    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create thread");
    }
    pthread_mutex_lock(&arr_mutex[0]);
    for (int i = 0; i < 21; ++i) {
        std::cout << "hello from main thread " << i << " times" << std::endl;
        pthread_mutex_lock(&arr_mutex[2]);
        pthread_mutex_unlock(&arr_mutex[0]);
        pthread_mutex_lock(&arr_mutex[1]);
        pthread_mutex_unlock(&arr_mutex[2]);
        pthread_mutex_lock(&arr_mutex[0]);
        pthread_mutex_unlock(&arr_mutex[1]);

    }
    pthread_mutex_unlock(&arr_mutex[0]);

    pthread_mutex_destroy(&arr_mutex[0]);
    pthread_mutex_destroy(&arr_mutex[2]);
    pthread_mutexattr_destroy(&pthreadMutexattr);
    pthread_exit(nullptr);
}
