#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

// создание двух потоков для эксперимента
void *thread_function(void *arg) {
    int thread_num = *(int *)arg;
    while(1) {
        if (thread_num == 1) {
            printf("oh");
            fflush(stdout);
        } else {
            printf("yeah");
            fflush(stdout);
        } 
        sleep(1);
    }
    
    return NULL;
}

// для определения политики и приоритета по умолчанию
int main() {
    pthread_t pthread1, pthread2;
    int num1 = 1, num2 = 2;
    
    pthread_create(&pthread1, NULL, thread_function, &num1);
    pthread_create(&pthread2, NULL, thread_function, &num2);
    
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);
    
    return 0;
}