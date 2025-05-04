#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/resource.h>

// поток fifo
void *fifo_thread(void *arg) {
    for (int i = 0; i < 100000; i++) {
        if (i % 10000 == 0){
            printf("F");
            fflush(stdout);
            usleep(10000);
            if (i == 90000) printf("\n");
        }
    }
    for (int i = 0; i < 5; i++) {
        printf("Step %d: process %s\n", i, "FIFO");
        sleep(1);
    }
    return NULL;
}

// поток rr
void *rr_thread(void *arg) {
    for (int i = 0; i < 100000; i++) {
        if (i % 10000 == 0) {
            printf("R");
            fflush(stdout);
            usleep(10000);
        }
    }
    for (int i = 0; i < 5; i++) {
        printf("Step %d: process %s\n", i, "RR");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t fifo_th, rr_th;
    
    pthread_create(&fifo_th, NULL, fifo_thread, NULL);
    pthread_create(&rr_th, NULL, rr_thread, NULL);
    
    // разные политики, один приоритет
    struct sched_param param = {.sched_priority = 50};
    pthread_setschedparam(fifo_th, SCHED_FIFO, &param);
    pthread_setschedparam(rr_th, SCHED_RR, &param);
    
    pthread_join(fifo_th, NULL);
    pthread_join(rr_th, NULL);
    
    return 0;
}