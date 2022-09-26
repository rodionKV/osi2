//
// Created by rodion on 17.09.22.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>

pthread_t ntid;

void* writer(void *arg) {
    std::ifstream input("/dev/urandom");
    for (std::string line; std::getline(input, line); )
    {
        std::cout<<line<<std::endl;
    }

    return ((void *)nullptr);
}

int main() {
    int err;

    err = pthread_create(&ntid, nullptr, writer, nullptr);
    if (err != 0) {
        perror("error create");
    }

    sleep(2);
    pthread_cancel(ntid);

    pthread_exit(nullptr);
}
