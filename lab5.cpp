//
// Created by rodion on 17.09.22.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <termios.h>

pthread_t ntid;
void finish(void * arg){
    std::cout<<"\nhello\n"<< std::endl;
}
void* writer(void *arg) {
    pthread_cleanup_push(finish, (void *) "\nhello\n");
    std::ifstream input("hello.txt");
    for (std::string line; std::getline(input, line); )
    {
        std::cout<<line<<std::endl;
    }
    pthread_cleanup_pop(1);
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
