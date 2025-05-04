#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/resource.h>

// работа потока
void *thread_function(void *arg) {
    int thread_num = *(int *)arg;
    const char* symbol = (thread_num == 1) ? "A" : "B";
    for (int i = 0; i<10; i++) {
        printf("%s", symbol);
        fflush(stdout);
    }
    
    return NULL;
}

// поток с изменяемым приоритетом
void* worker_thread(int prio, int prio1, int prio2) {
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);

    // SCHED_FIFO с различными приоритетами
    struct sched_param param = {.sched_priority = prio};
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    struct sched_param param1 = { .sched_priority = prio1 };
    pthread_setschedparam(pthread1, SCHED_FIFO, &param1);
    struct sched_param param2 = { .sched_priority = prio2 };
    pthread_setschedparam(pthread2, SCHED_FIFO, &param2);

    for (int i = 0; i<10; i++) {
        printf("C");
        fflush(stdout);
    }

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    return NULL;
}

// проведение экспериментов
int main() {
    printf("5.3.1: одинаковые приоритеты\n");
    printf("Все приоритеты по 50\n");
    worker_thread(50,50,50);

    printf("\n\n5.3.2: Определяем границы приоритетов для FIFO\n");
    int min_prio = sched_get_priority_min(SCHED_FIFO);
    int max_prio = sched_get_priority_max(SCHED_FIFO);
    printf("SCHED_FIFO priority range: %d (min) - %d (max)\n", min_prio, max_prio);

    printf("\n5.3.3: разные приоритеты\n");
    printf("Приоритеты: 30 - A(1), 70 - B(2), 50 - C(main)\n");
    worker_thread(50,30,70);
    printf("\n");

    return 0;
}