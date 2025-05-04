#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// пользовательский обработчик сигнала, выводит строку
void sigusr1_handler(int sig) {
    printf("son3 (PID: %d) received SIGUSR1 signal\n", getpid());
}

int main() {
    printf("son3 (PID: %d) started - custom handler\n", getpid());
    
    signal(SIGUSR1, sigusr1_handler);
    
    while(1) {
        sleep(5);
    }
    
    return 0;
}