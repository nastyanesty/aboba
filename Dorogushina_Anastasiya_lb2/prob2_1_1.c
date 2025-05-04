#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    // новый процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    //переменная для вычислений
    int check = 0;
    if (pid == 0) {
        // выполнение процесса-потомка
        printf("Child process:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        for (int i = 0; i < 5; i++) check += i;
        printf("1+2+3+4 = %d\n", check);
    } else {
        // выполнение процесса-родителя
        printf("Parent process:\n");
        printf("PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        for (int i = 0; i < 5; i++) check += i*i;
        printf("1^2+2^2+3^2+4^2 = %d\n", check);
        // ожидание процесса-потомка
        int status;
        wait(&status);
    }

    printf("Process finished (PID: %d)\n", getpid());
    return 0;
}