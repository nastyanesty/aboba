#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <string.h>

#define STACK_SIZE (1024 * 1024)
#define BUF_SIZE 100

// обработчик
void signal_handler(int sig) {
    printf("PID %d received SIGUSR1 signal\n", getpid());
}

// функция потомка
int child_function(void *arg) {
    int *file_descriptor = (int *)arg;
    const char *message = "Child wrote\n";
    
    write(*file_descriptor, message, strlen(message));
    
    printf("Child PID=%d, nice=%d, scheduling policy=%d\n", getpid(), getpriority(PRIO_PROCESS, 0),sched_getscheduler(0));
    
    raise(SIGUSR1);
    return 0;
}

int main() {
    // файл
    int file_fd = open("prob6/4.txt", O_CREAT|O_RDWR|O_TRUNC, 0644);
    if (file_fd == -1) {
        perror("Failed to open file");
        exit(1);
    }
    
    // приоритет и политика планирования
    nice(5);
    struct sched_param param = {0};
    if (sched_setscheduler(0, SCHED_OTHER, &param) == -1) {
        perror("Failed to set scheduler");
    }
    
    // обработчик сигнала
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("Failed to set signal handler");
    }
    
    // память для стека
    char *child_stack = malloc(STACK_SIZE);
    if (!child_stack) {
        perror("Failed to allocate stack");
        close(file_fd);
        exit(1);
    }
    
    // клонированный процесс
    pid_t child_pid = clone(child_function, child_stack + STACK_SIZE, CLONE_FILES | CLONE_SIGHAND | CLONE_VM | SIGCHLD, &file_fd);
    if (child_pid == -1) {
        perror("Clone failed");
        free(child_stack);
        close(file_fd);
        exit(1);
    }
    
    sleep(1);
    
    // чтение из файла
    lseek(file_fd, 0, SEEK_SET);
    char buffer[BUF_SIZE];
    ssize_t bytes_read = read(file_fd, buffer, BUF_SIZE - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Чтение из файла: %s", buffer);
    }
    
    close(file_fd);
    free(child_stack);
    
    return EXIT_SUCCESS;
}