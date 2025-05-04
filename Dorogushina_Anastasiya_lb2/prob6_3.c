#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

// обработчик сигнала
void handler(int sig) {
    printf("Signal %d received in %d\n", sig, getpid());
}

// наследование сигнала
int main() {
    signal(SIGUSR1, handler);
    printf("Parent with pid = %d\n", getpid());
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child with pid = %d\n", getpid());
        system("ps -s");
        pause();
    } else {
        sleep(1);
        kill(pid, SIGUSR1);
        wait(NULL);
    }
    return 0;
}