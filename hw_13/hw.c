#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    char prev_file[10] = "0.txt";
    char file[10] = "1";
    char buf[9] = "some text";

    int fd = open(prev_file, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        printf("Не удалось создать файл.\n");
        return 1;
    }
    write(fd, buf, 9);
    close(fd);

    int depth = 1;
    while (1) {
        if (symlink(prev_file, file) == -1) {
            printf("Ошибка создание связи. Глубина рекурсивных ссылок для для hard-link связей: %d\n", depth);
            return 1;
        }
        fd = open(file, O_RDONLY);
        if (fd == -1) {
            printf("Ошибка. Файл нельзя открыть");
            break;
        }
        close(fd);
        sprintf(prev_file, "%d", depth);
        ++depth;
        sprintf(file, "%d", depth);
    }
    printf("Глубина рекурсивных ссылок для hard-link связей: %d\n", depth);
    return 0;
}