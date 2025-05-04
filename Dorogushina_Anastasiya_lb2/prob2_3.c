#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

// обработка ошибок
void exec_error(const char* func_name) {
    perror(func_name);
    exit(1);
}

// текущие процессы
void log_processes(int test_num) {
    char cmd[100];
    sprintf(cmd, "ps -f --ppid %d > prob2/test_%d_processes.log", getpid(), test_num);
    system(cmd);
}

// тестирование
void run_exec_test(int test_num) {
    // новый процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }
    // процесс-потомок
    if (pid == 0) {
        printf("\nТест %d:", test_num);
        
        // для exec
        char* program = "/bin/ls";
        char* args[] = {"ls", "-l", "-a", NULL}; // -l -a
        
        // окружение
        char* custom_env[] = {"TEST_ENV=123", "LANG=C", NULL};
        
        // сами тесты
        switch(test_num) {
            case 0:
                printf("execl()\n");
                execl(program, "ls", "-l", "-a", NULL);
                exec_error("execl");
                break;
                
            case 1:
                printf("execlp()\n");
                execlp("ls", "ls", "-l", "-a", NULL);
                exec_error("execlp");
                break;
                
            case 2:
                printf("execle()\n");
                execle(program, "ls", "-l", "-a", NULL, custom_env);
                exec_error("execle");
                break;
                
            case 3:
                printf("execv()\n");
                execv(program, args);
                exec_error("execv");
                break;
                
            case 4:
                printf("execvp()\n");
                execvp("ls", args);
                exec_error("execvp");
                break;
                
            default:
                fprintf(stderr, "Неизвестный тест\n");
                exit(1);
        }
    } else {
        // процесс-родитель
        log_processes(test_num);

        // ожидание завершения потомка
        int status;
        waitpid(pid, &status, 0);
        
        // статус завершения
        if (WIFEXITED(status)) {
            printf("Тест %d: дочерний процесс завершился со статусом %d\n", 
                   test_num, WEXITSTATUS(status));
        } else {
            printf("Тест %d: дочерний процесс завершился с ошибкой!\n", test_num);
        }
    }
}

int main() {
    printf("Эксперименты с семейством exec()\n");
    
    for (int i = 0; i < 5; i++) {
        run_exec_test(i);
    }
    
    printf("\nВсе тесты завершены\n");
    return 0;
}