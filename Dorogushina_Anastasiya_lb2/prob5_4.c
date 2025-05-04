#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/resource.h>
#include <time.h>
#include <stdlib.h>

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
void* worker_thread(int prio, int prio1, int prio2, int yield) {
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);

    // SCHED_RR с различными приоритетами
    struct sched_param param = {.sched_priority = prio};
    pthread_setschedparam(pthread_self(), SCHED_RR, &param);
    struct sched_param param1 = { .sched_priority = prio1 };
    pthread_setschedparam(pthread1, SCHED_RR, &param1);
    struct sched_param param2 = { .sched_priority = prio2 };
    pthread_setschedparam(pthread2, SCHED_RR, &param2);

    for (int i = 0; i<10; i++) {
        printf("C");
        fflush(stdout);
        if (yield) sched_yield();
    }

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    return NULL;
}

// проведение экспериментов
int main() {
    printf("5.4.1: одинаковые приоритеты\n");
    printf("Все приоритеты по 50, sched_yield = 0\n");
    worker_thread(50,50,50,0);
    printf("\nВсе приоритеты по 50, sched_yield = 1\n");
    worker_thread(50,50,50,1);

    printf("\n\n5.4.2: Определяем границы приоритетов для RR\n");
    int min_prio = sched_get_priority_min(SCHED_RR);
    int max_prio = sched_get_priority_max(SCHED_RR);
    printf("SCHED_RR priority range: %d (min) - %d (max)\n", min_prio, max_prio);

    printf("\n5.4.3: разные приоритеты\n");
    printf("Приоритеты: 30 - A(1), 70 - B(2), 50 - C(main)\n");
    worker_thread(50,30,70,0);
    printf("\nТе же приоритеты, sched_yield = 1\n");
    worker_thread(50,30,70,1);
    printf("\n");
    return 0;
}