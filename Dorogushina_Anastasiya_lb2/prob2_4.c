#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

// текущие процессы
void log_process_table() {
    printf("Текущие процессы:\n");
    system("ps -o pid,ppid,state,comm | grep -E 'a.out'");
}

// родитель ждет завершения потомка
void a() {
    printf("Запуск процесса-потомка и ожидание завершения\n");
    pid_t pid = fork();
    if (pid == 0) {
        // процесс-потомок
        sleep(2);
        sleep(2);
        printf("Потомок завершил работу\n");
        exit(0);
    } else {
        // процесс-родитель
        printf("Родитель ожидает завершения потомка (PID родителя: %d)...\n", getpid());
        log_process_table();
        wait(NULL);
        printf("Родитель: потомок завершился, родитель выходит\n");
        log_process_table();
    }
}

// родитель завершается раньше потомка
void b() {
    printf("\nЗавершение родителя перед потомком\n");
    pid_t pid = fork();
    if (pid == 0) {
        int initial_ppid = getppid();
        printf("Потомок запущен (PID: %d, Исходный PPID: %d)\n", getpid(), initial_ppid);
        
        // пока родитель не завершится
        while(getppid() == initial_ppid) {
            usleep(100000);
        }
        
        int new_ppid = getppid();
        printf("Новый PPID потомка после завершения родителя: %d\n", new_ppid);
        
        log_process_table();
        exit(0);
    } else {
        printf("PID родителя: %d, завершается сейчас...\n", getpid());
        exit(0);
    }
}

// демонстрация процесса-зомби
void c() {
    printf("\nПоявление процесса-зомби\n");
    
    pid_t pid = fork();
    if (pid == 0) {
        printf("Потомок запущен (PID: %d, станет зомби)\n", getpid());
        exit(0);
    } else {
        printf("Родитель не ожидает потомка (PID: %d)...\n", getpid());
        
        sleep(2);
        printf("Существует процесс-зомби:\n");
        log_process_table();
        
        sleep(2);
        wait(NULL);
        
        printf("После wait() - зомби удален:\n");
        log_process_table();
        printf("Родитель завершен\n");
    }
}

int main() {
    a();
    if (fork() == 0) {
        b();
        exit(0);
    }
    wait(NULL);
    sleep(10);
    c();
    return 0;
}