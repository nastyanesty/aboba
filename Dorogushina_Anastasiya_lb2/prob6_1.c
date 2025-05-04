#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// наследование доступа к файлу
int main() {
    // файл для записи
    FILE* fd = fopen("prob6/1.txt", "w");
    fprintf(fd, "Parent before child\n");
    fflush(fd);

    pid_t pid = fork();
    if (pid == 0) {
        fprintf(fd, "Child before exec\n");
        fflush(fd);
        printf("Перед execl:\n");
        system("cat prob6/1.txt");
        execl("/bin/sleep", "sleep", "2", NULL);
    } else {
        // ожидание завершения потомка
        wait(NULL);
        // переоткрытие на чтение и чтение файла
        fclose(fd);
        fd = fopen("prob6/1.txt", "r");
        printf("Чтение после execl:\n");
        char buf[100];
        size_t bytes_read = fread(buf, 1, sizeof(buf), fd);
        buf[bytes_read] = '\0';
        printf("%s", buf);
        fclose(fd);
    }

    return 0;
}