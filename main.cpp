#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;

void* writer(void * agr) {
    execl("/bin/cat", "cat", "main.cpp", (char *) 0);

    sleep(100);
    return ((void *)nullptr);
}

int main() {
    int err;

    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create");
    }
   // execl("/bin/cat","main.cpp",NULL );
    sleep(100);
    pthread_join(ntid, nullptr);
    pthread_exit(nullptr);
}
