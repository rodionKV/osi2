#include <iostream>
#include <pthread.h>

pthread_t ntid;

void* writer(void *arg) {
    for (int i = 1; i < 21; ++i) {
        std::cout << "hello from second thread " << i << " times" << std::endl;
    }
    return ((void *)nullptr);
}

int main() {
    int err;

    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create");
    }

    for (int i = 1; i < 11; ++i) {
        std::cout << "hello from main thread " << i << " times" << std::endl;
    }

    pthread_exit(nullptr);
}
