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

    int sockfd;
    int port = atoi(argv[2]);

    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE];

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("opening socket");

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(port);

    // Установка соединения с сервером
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("connecting to server");
    }

    printf("Connected to server\n");

    // Прием сообщений от сервера
    while (1) {
        memset(buff, 0, BUFF_SIZE);

        // Чтение сообщения от сервера
        ssize_t read_size = read(sockfd, buff, BUFF_SIZE);
        if (read_size < 0) {
            error("read from server");
        }

        printf("Received: %s", buff);

        // Проверка на завершение работы
        if (strcmp(buff, "The End\n") == 0) {
            break;
        }
    }

    // Закрытие соединения и сокета
    close(sockfd);
    return 0;
}
