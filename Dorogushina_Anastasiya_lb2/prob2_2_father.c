#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    // процесс-родитель
    printf("Parent process:\n");
    printf("PID: %d\n", getpid());
    //список процессов в файле
    system("ps -l > prob2/2_father_start.txt");

    // новый процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    // процесс-потомок
    if (pid == 0) {
        // образ процесса - программа prob2_2_son
        execl("./son", "./son", NULL);
        perror("execl failed");
        exit(1);
    } else {
        // список процессов в файле
        system("ps -l > prob2/2_father_son.txt");
        wait(NULL);
        // список процессов после завершения потомка
        system("ps -l > prob2/2_father_stop.txt");
        printf("Parent process finished\n");
    }

    return 0;
}