#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// процессы и сигналы
int main() {
    printf("Father process started (PID: %d)\n", getpid());
    pid_t son1, son2, son3;
    // 3 процесса-потомка
    if ((son1 = fork()) == 0) {
        execl("./son_1", "son1", NULL);
    }
    if ((son2 = fork()) == 0) {
        execl("./son_2", "son2", NULL);
    }
    if ((son3 = fork()) == 0) {
        execl("./son_3", "son3", NULL);
    }
    sleep(2);
    // таблица процессов
    system("ps -s > prob3/start.txt");
    
    // отправка сигналов
    printf("\nFather: Sending SIGUSR1 to son1 (default handler)\n");
    kill(son1, SIGUSR1);
    printf("Father: Sending SIGUSR1 to son2 (ignore)\n");
    kill(son2, SIGUSR1);
    printf("Father: Sending SIGUSR1 to son3 (custom handler)\n");
    kill(son3, SIGUSR1);

    // таблица процессов
    sleep(2);
    system("ps -s > prob3/stop.txt");

    // заканчиваем все процессы-потомки
    printf("\nFather: Stop son1, son2, son3\n");
    kill(son1, SIGTERM);
    kill(son2, SIGTERM);
    kill(son3, SIGTERM);
    while (wait(NULL) > 0);

    printf("Father process finished\n");
    // вывод всего, записанного в файлы
    printf("\nBefore signal:\n");
    system("cat prob3/start.txt");
    printf("\nAfter signal:\n");
    system("cat prob3/stop.txt");
    return 0;
}