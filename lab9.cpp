#include <iostream>
#include <vector>
#include <malloc.h>
#include <signal.h>

#define num_steps 200000000
#define num_steps_pause 1000000

#define num_thread 5

struct info {
    int first;
    int size_step;
};
struct result {
    double decimal;
};
bool stopcheck = false;
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

static void sig_int(int signo) {
    if (signo == SIGINT)
        stopcheck = true;
}

void *writer(void *agr) {
    struct result *result = static_cast<struct result *>(malloc(sizeof(struct result)));
    //double pi_part = 0;
    long long i;
    struct info structinfo = *static_cast<struct info *>(agr);

    result->decimal = 0;

    for (i = structinfo.first;; i += structinfo.size_step) {
        if (((i - structinfo.first) % num_steps_pause) == 0) {
            pthread_barrier_wait(&barrier1);
            if (stopcheck) {
                pthread_barrier_wait(&barrier2);
                pthread_exit((void *) result);
            }
        }
        result->decimal += 1.0 / (i * 4.0 + 1.0);
        result->decimal -= 1.0 / (i * 4.0 + 3.0);
    }


}

int main(int argc, char **argv) {
    std::vector<pthread_t> threads;
    pthread_barrier_init(&barrier1, nullptr, num_thread);
    pthread_barrier_init(&barrier2, nullptr, num_thread);
    for (int j = 0; j < num_thread; ++j) {
        pthread_t pthread;
        auto *structinfo = static_cast<info *>(calloc(sizeof(struct info), 1));
        structinfo->first = j;
        structinfo->size_step = num_thread;
        int err;

        err = pthread_create(&pthread, nullptr, writer, (void *) structinfo);


        if (err != 0) {
            perror("error create");
        }

        threads.push_back(pthread);

    }
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        perror("невохможно перехватить сигнал");
    }
    double result = 0;
    for (int j = 0; j < num_thread; ++j) {
        struct result *part_result;

        pthread_join(threads[j], (void **) (&part_result));

        result += part_result->decimal;
    }


    result *= 4.0;
    printf("pi done - %.15g \n", result);

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

    pthread_exit(nullptr);
}
