#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <signal.h>

#define STACK_SIZE (1024 * 1024)

// добавлено для 4.3., остальное как в 4.1.
int shared_counter = 0;
FILE* file;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg) {
    int thread_num = *(int *)arg;
    pthread_mutex_lock(&mutex);
    printf("Поток pthread %d: PID=%d, TID=%ld, shared_counter=%d\n", 
           thread_num, getpid(), syscall(SYS_gettid), shared_counter);
    shared_counter++;
    fprintf(file, "Поток pthread %d завершается, shared_counter=%d\n", 
           thread_num, shared_counter);
    fflush(file);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int clone_function(void *arg) {
    int thread_num = *(int *)arg;
    pthread_mutex_lock(&mutex);
    printf("Поток clone %d: PID=%d, TID=%ld, shared_counter=%d\n", 
           thread_num, getpid(), syscall(SYS_gettid), shared_counter);
    shared_counter++;
    fprintf(file, "Поток clone %d завершается, shared_counter=%d\n", 
           thread_num, shared_counter);
    fflush(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main() {
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    file = fopen("prob4/res.txt", "w");
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);
    
    void *clone_stack1 = malloc(STACK_SIZE);
    void *clone_stack2 = malloc(STACK_SIZE);
    
    int num3 = 3, num4 = 4;
    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    
    clone(clone_function, clone_stack1 + STACK_SIZE, flags, &num3);
    clone(clone_function, clone_stack2 + STACK_SIZE, flags, &num4);
    
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    
    sleep(3);
    
    free(clone_stack1);
    free(clone_stack2);
    pthread_mutex_destroy(&mutex);
    fclose(file);
    
    printf("Основной поток завершен, shared_counter=%d\n", shared_counter);
    
    printf("\nres.txt:\n");
    system("cat prob4/res.txt");

    return 0;
}