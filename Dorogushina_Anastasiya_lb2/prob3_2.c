#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t child1, child2, child3;

    // 3 процесса-потомка
    if ((child1 = fork()) == 0) {
        printf("Child1 (PID: %d) started\n", getpid());
        sleep(2);
        exit(10);
    }

    if ((child2 = fork()) == 0) {
        printf("Child2 (PID: %d) started\n", getpid());
        sleep(4);
        exit(20);
    }

    if ((child3 = fork()) == 0) {
        printf("Child3 (PID: %d) started\n", getpid());
        sleep(6);
        exit(30);
    }

    int status;
    pid_t waited_pid;

    // ожидание завершения child2
    waited_pid = waitpid(child2, &status, 0);
    if (waited_pid)
        printf("Child2 (PID: %d) exited with code %d\n", waited_pid, WEXITSTATUS(status));

    // ожидание завершения child3
    waited_pid = waitpid(child3, &status, 0);
    if (waited_pid>0)
        printf("Child3 (PID: %d) exited by signal %d\n", waited_pid, WEXITSTATUS(status));

    // ожидание завершения child1
    waited_pid = waitpid(child1, &status, 0);
    if (waited_pid)
        printf("Child1 (PID: %d) exited by signal %d\n", waited_pid, WEXITSTATUS(status));
    
    return 0;
}