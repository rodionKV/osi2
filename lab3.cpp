
#include <stdio.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>

#define LINE_SIZE 20
#define COUNT_THREAD 4

struct lines {
    std::vector<char *> list;
};

void *write_agr(void *arguments) {
    pthread_t pthread;
    pthread = pthread_self();
    std::cout << "write " << pthread << " thread :" << std::endl;
    auto *lines1 = static_cast<lines *>(arguments);
    for (auto &line: lines1->list) {
        std::cout << line << std::endl;
        free(line);
    }
    free(arguments);
    return (void *) nullptr;
}

int main() {
    char command[BUFSIZ];
    sprintf(command, "tr -dc A-Za-z0-9 </dev/urandom");
    FILE *fpin = popen(command, "r");

    std::vector<pthread_t> threads(COUNT_THREAD);
    for (int i = 0; i < COUNT_THREAD; ++i) {
        int err;
        pthread_t pthread;

        auto *lines1 = static_cast<lines *>(calloc(sizeof(struct lines), 1));

        for (int j = 0; j < rand()%10+1; ++j) {
            char *line = static_cast<char *>(calloc(sizeof(char), LINE_SIZE));
            fgets(line, LINE_SIZE + 1, fpin);
            lines1->list.push_back(line);
        }

        err = pthread_create(&pthread, nullptr, write_agr, (void *) lines1);

        if (err != 0)
            perror("error create thread");
    }

    pclose(fpin);
    pthread_exit(nullptr);
}