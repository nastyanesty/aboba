#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/resource.h>

// наследование приоритета и политики
int main() {
    // установка приоритета и политики родителю
    struct sched_param param;
    param.sched_priority = 20;
    sched_setscheduler(0, SCHED_RR, &param);
    // создание потомка
    pid_t pid = fork();
    if (pid == 0) {
        // вывод информации о потомке
        int policy = sched_getscheduler(0);
        printf("Child PID: %d\n", getpid());
        sched_getparam(0, &param);
        printf("Child priority: %d\n", param.sched_priority);
        printf("Child scheduling policy: %s\n",
            (policy == SCHED_OTHER) ? "SCHED_OTHER" :
            (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
            (policy == SCHED_RR)    ? "SCHED_RR" : "Unknown");
        execl("/bin/ls", "ls", NULL);
    } else {
        // вывод информации о родителе
        int policy = sched_getscheduler(0);
        printf("Parent PID: %d\n", getpid());
        param.sched_priority = 0;
        sched_getparam(0, &param);
        printf("Parent priority: %d\n", param.sched_priority);
        printf("Parent scheduling policy: %s\n",
            (policy == SCHED_OTHER) ? "SCHED_OTHER" :
            (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
            (policy == SCHED_RR)    ? "SCHED_RR" : "Unknown");
        wait(NULL);
    }
    return 0;
}