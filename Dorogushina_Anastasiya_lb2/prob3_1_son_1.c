#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// обычная обработка сигнала по умолчанию
int main() {
    printf("son1 (PID: %d) started - default handler\n", getpid());
    while(1) {
        sleep(5);
    }
    return 0;
}