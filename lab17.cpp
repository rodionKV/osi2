#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_t ntid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct list_item {
    struct list_item *after;
    struct list_item *before;
    std::string *string;
};
struct list_item *top_list = nullptr;

void add(std::string *str) {
    pthread_mutex_lock(&mutex);
    auto *item = (struct list_item *) malloc(sizeof(struct list_item));
    item->string = str;
    item->after = top_list;
    top_list = item;
    pthread_mutex_unlock(&mutex);
}

int comp(const std::string &a, const std::string &b) {
    return a.compare(b);
}

void swap(struct list_item *a, struct list_item *b) {
    struct list_item *a_before = a->before;
    struct list_item *b_after = b->after;

    if (a_before == nullptr) {
        top_list = b;
    } else {
        a_before->after = b;
    }
    a->after = b_after;
    a->before = b;
    b->after = a;
    b->before = a_before;
    if (b_after != nullptr) {
        b_after->before = a;
    }

}

void sort() {
    pthread_mutex_lock(&mutex);
    bool flag = false;
    do {
        flag = false;
        struct list_item *current = top_list;
        while (current != nullptr && current->after != nullptr) {
            struct list_item *next = current->after;
            if (comp(*(current->string), *(next->string)) > 0) {
                flag = true;
                swap(current, next);
            } else {
                current = current->after;
            }

        }
    } while (flag);
    pthread_mutex_unlock(&mutex);
}

void print() {
    pthread_mutex_lock(&mutex);
    struct list_item *copy = top_list;
    std::cout << "#start print#" << std::endl;
    while (copy != nullptr) {
        std::cout << *copy->string << std::endl;
        copy = copy->after;
    }
    std::cout << "#finish print#" << std::endl;
    pthread_mutex_unlock(&mutex);
}

void *writer(void *arg) {
    while (true) {
        sort();
        sleep(5);
    }
    return ((void *) nullptr);
}

void del_arr() {
    pthread_mutex_lock(&mutex);
    std::cout << "#delete start#" << std::endl;
    while (top_list != nullptr) {
        struct list_item *copy_top_list = top_list;
        free(top_list->string);
        top_list = top_list->after;
        free(copy_top_list);
    }
    std::cout << "#delete completed#" << std::endl;
}

static void sig_usr(int signo) {
    if (signo == SIGINT) {
        if (pthread_cancel(ntid) == 0) {
            std::cout << "#thread success stop#" << std::endl;
        }
        del_arr();
        exit(0);
    }
}


int main() {

    signal(SIGINT, sig_usr);
    int err;

    err = pthread_create(&ntid, nullptr, writer, nullptr);

    if (err != 0) {
        perror("error create");
    }

    for (std::string line; std::getline(std::cin, line);) {
        if (line == " ") {
            print();
            continue;
        }

        auto *str = (std::string *) malloc(sizeof(std::string));
        *str = line;
        add(str);
    }

    pthread_exit(nullptr);
}
