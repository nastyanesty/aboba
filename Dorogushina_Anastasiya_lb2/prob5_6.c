#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

// работа потока
void* fn(void* arg) {
    int id = *(int*)arg;
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    const char* policy_name = (policy == SCHED_OTHER) ? "OTHER" : (policy == SCHED_RR ? "RR" : "FIFO");
    
    for (int i = 0; i < 3; i++) {
        printf("Step %d: process %d - %s, %d\n", i, id, policy_name, param.sched_priority);
        sleep(3);
    }
    return NULL;
}

// проставление нужных параметров: политика и приоритет для потоков
void exper(const char* desc, int policies[], int priorities[]) {
    printf("\n%s\n", desc);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    pthread_t threads[3];
    pthread_attr_t attrs[3];
    int ids[] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        pthread_attr_init(&attrs[i]);
        
        int prio = (priorities == NULL) ? 1 : priorities[i];
        struct sched_param param = {.sched_priority = prio};
        pthread_attr_setinheritsched(&attrs[i], PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attrs[i], policies[i]);
        pthread_attr_setschedparam(&attrs[i], &param);
        
        pthread_create(&threads[i], &attrs[i], fn, &ids[i]);
        pthread_setaffinity_np(threads[i], sizeof(cpuset), &cpuset);
    }
    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }
}

// эксперименты с политикой и приоритетом потоков одного процесса
int main() {
    printf("Политика по умолчанию, приоритеты тоже\n");
    pthread_t t1;
    int id = 1;
    pthread_create(&t1, NULL, fn, &id);
    pthread_join(t1, NULL);

    int policies[] = {SCHED_OTHER, SCHED_RR, SCHED_OTHER};
    exper("Один RR, два OTHER", policies, NULL);

    policies[2] = SCHED_RR;
    policies[1] = SCHED_FIFO;
    policies[0] = SCHED_FIFO;
    exper("Один RR, два FIFO", policies, NULL);

    policies[0] = SCHED_OTHER;
    int priorities[] = {1, 25, 50};
    exper("Разные приоритеты, разная политика", policies, priorities);

    priorities[2] = 1;
    exper("Два одинаковых приоритета, разная политика", policies, priorities);

    policies[2] = SCHED_OTHER;
    policies[0] = SCHED_RR;
    exper("Смена OTHER на RR и наоборот, приоритеты одинаковы", policies, priorities);
    return 0;
}