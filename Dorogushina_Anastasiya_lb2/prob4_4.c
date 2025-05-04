#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

void *thread_function(void *arg) {
    int thread_num = *(int *)arg;
    printf("Поток pthread %d: PID=%d, TID=%ld\n", thread_num, getpid(), syscall(SYS_gettid));
    
    while(1) {
        printf("%d working\n", thread_num);
        sleep(1);
    }
    
    return NULL;
}

// создание абсолютно обычных нитей для эксперимента
int main() {
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);
    
    printf("Главный поток (PID: %d)\n", getpid());
    
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    
    return 0;
}