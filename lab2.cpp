#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;

void* writer(void *arg) {
    for (int i = 1; i < 11; ++i) {
        std::cout << "hello from second thread " << i << " times" << std::endl;
    }
    //sleep(1000);
    return ((void *)0);
}

int main() {
    int err;

    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create thread");
    }
    pthread_join(ntid,nullptr);

    for (int i = 1; i < 11; ++i) {
        std::cout << "hello from main thread " << i << " times" << std::endl;
    }

    pthread_exit(nullptr);
}
