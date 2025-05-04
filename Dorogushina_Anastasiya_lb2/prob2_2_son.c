#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // процесс-потомок
    printf("Child process:\n");
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    // список процессов в файле
    system("ps -l > prob2/2_son_start.txt");
    // вычисление 2
    int num = 64;
    for (int i = 0; i<5; i++) num /= 2;
    char command[100];
    snprintf(command, sizeof(command), "ps -l > prob2/%d_son_stop.txt", num);
    // список процессов в файле
    system(command);

    printf("Child process finished\n");
    return 0;
}