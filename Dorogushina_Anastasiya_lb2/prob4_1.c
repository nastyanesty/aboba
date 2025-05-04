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

// стек для потоков
#define STACK_SIZE (1024 * 1024)

// функция для потоков через pthread_create
void *thread_function(void *arg) {
    int thread_num = *(int *)arg;
    printf("Поток pthread %d: PID=%d, TID=%ld\n", 
           thread_num, getpid(), syscall(SYS_gettid));
    sleep(3);
    printf("Поток pthread %d завершается\n", 
           thread_num);
    return NULL;
}

// функция для потоков через clone
int clone_function(void *arg) {
    int thread_num = *(int *)arg;
    printf("Поток clone %d: PID=%d, TID=%ld\n", 
           thread_num, getpid(), syscall(SYS_gettid));
    sleep(3);
    printf("Поток clone %d завершается\n", 
           thread_num);
    return 0;
}

int main() {
    // создание потоков
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    char cmd1[100];
    sprintf(cmd1, "ps -T -p %d -o pid,tid,cmd,psr,pcpu", getpid());
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);
    
    // стек для потоков clone
    void *clone_stack1 = malloc(STACK_SIZE);
    void *clone_stack2 = malloc(STACK_SIZE);
    
    // флаги для clone
    int num3 = 3, num4 = 4;
    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    
    clone(clone_function, clone_stack1 + STACK_SIZE, flags, &num3);
    clone(clone_function, clone_stack2 + STACK_SIZE, flags, &num4);
    
    sleep(1);
    // вывод информации
    printf("\nПотоки процесса текущие:\n");
    system(cmd1);
    printf("\n");

    // ожидание завершения потоков
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    sleep(3);
    
    free(clone_stack1);
    free(clone_stack2);
    
    printf("Основной поток завершен\n");
    printf("\nПотоки процесса после:\n");
    system(cmd1);
    return 0;
}