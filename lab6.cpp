//
// Created by rodion on 17.09.22.
//
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <unistd.h>

#define COEFFICIENT 1e3

void* writer(void *arg) {
    std::string  str = *static_cast<std::string *>(arg);
    long sleep_time = str.size()*COEFFICIENT;
    usleep(sleep_time);
    std::cout<<str<<std::endl;
    return ((void *)nullptr);
}

int main() {
    std::vector<std::pair<std::string,pthread_t>> lines;
    std::string str;
    while (std::getline(std::cin, str)) {
        pthread_t pthread;
        std::pair<std::string,pthread_t> line (str,pthread);
        lines.push_back(line);
    }

    std::cout << "Result:" << std::endl;

    for (auto & line : lines) {

        int err;

        err = pthread_create(&line.second, nullptr, writer, &line.first);

        if (err != 0) {
            perror("error create");
        }
    }
    pthread_exit(nullptr);

}