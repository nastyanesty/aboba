#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// игнорирование сигнала
int main() {
    printf("son2 (PID: %d) started - ignore\n", getpid());
    
    signal(SIGUSR1, SIG_IGN);
    
    while(1) {
        sleep(5);
    }
    
    return 0;
}