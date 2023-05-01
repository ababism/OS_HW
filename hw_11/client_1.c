#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 1024

void error(const char *message) {
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    int socket_fd;
    int port = atoi(argv[2]);

    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE];

    // Создание сокета
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        error("opening socket");

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(port);

    // Установка соединения с сервером
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Error connecting to server");

    printf("Connected to server\n");

    // Отправка сообщений на сервер
    while (1) {
        memset(buff, 0, BUFF_SIZE);

        // Чтение сообщения с клавиатуры
        fgets(buff, BUFF_SIZE, stdin);

        // Отправка сообщения на сервер
        ssize_t write_size = write(socket_fd, buff, strlen(buff));
        if (write_size < 0)
            error("write to server");

        // Проверка на завершение работы
        if (strcmp(buff, "The End\n") == 0)
            break;
    }

    // Закрытие соединения и сокета
    close(socket_fd);

    return 0;
}
