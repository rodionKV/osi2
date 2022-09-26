#include <iostream>
#include <vector>
#include <malloc.h>

#define num_steps 200000000

#define num_thread 2

struct info {
    int first;
    int size_step;
};
struct result {
    double decimal;
};

void *writer(void *agr) {
    struct result *result = static_cast<struct result *>(malloc(sizeof(struct result)));
    //double pi_part = 0;
    int i;
    struct info structinfo = *static_cast<struct info *>(agr);

    result->decimal = 0;
    // std::cout<<structinfo.first<<" "<<structinfo.size_step<<std::endl;
    for (i = structinfo.first; i < num_steps; i += structinfo.size_step) {

        result->decimal += 1.0 / (i * 4.0 + 1.0);
        result->decimal -= 1.0 / (i * 4.0 + 3.0);
    }
    std::cout << "thread:" << result << std::endl;
    pthread_exit((void *) result);
}

int main(int argc, char **argv) {
    std::vector<pthread_t> threads;

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
    double result = 0;
    for (int j = 0; j < num_thread; ++j) {
        struct result *part_result;

        pthread_join(threads[j], ((void **) part_result));
        std::cout << part_result << std::endl;
        result += part_result->decimal;
    }


    result *= 4.0;
    printf("pi done - %.15g \n", result);

    pthread_exit(nullptr);
}
