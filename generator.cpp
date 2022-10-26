
#include <stdio.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>

#define LINE_SIZE 20


int main() {
    char command[BUFSIZ];
    sprintf(command, "tr -dc A-Za-z0-9 </dev/urandom");
    FILE *fpin = popen(command, "r");

    char *line = static_cast<char *>(calloc(sizeof(char), LINE_SIZE));
    int n = 0;
    while (true) {
        sleep(1);
        n++;
        fgets(line, LINE_SIZE - 1, fpin);
        std::cout << line << std::endl;
        if (n % 10) {
            std::cout << " " << std::endl;
        }
    }

    pclose(fpin);
    pthread_exit(nullptr);
}