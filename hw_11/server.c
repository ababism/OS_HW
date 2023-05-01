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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int server_s_fd, client_1_socket, client_2_socket;
    int port = atoi(argv[1]);

    struct sockaddr_in server_addr, client_1_addr, client_2_addr;
    socklen_t client_1_len, client_2_len;
    char buff[BUFF_SIZE];

    // Создание сокета
    server_s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_s_fd < 0) {
        error("opening socket");
    }
    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Привязка сокета к адресу сервера
    if (bind(server_s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("bind");
    }

    // Ожидание клиентов
    listen(server_s_fd, 5);
    printf("Server initialized. Waiting for clients...\n");

    // Принятие первого клиента
    client_1_len = sizeof(client_1_addr);
    client_1_socket = accept(server_s_fd, (struct sockaddr *)&client_1_addr, &client_1_len);
    if (client_1_socket < 0) {
        error("Error on accept");
    }

    printf("Client_1 connected\n");

    // Принятие второго клиента
    client_2_len = sizeof(client_2_addr);
    client_2_socket = accept(server_s_fd, (struct sockaddr *)&client_2_addr, &client_2_len);
    if (client_2_socket < 0) {
        error("Error on accept");
    }

    printf("Client_2 connected\n");

    // Пересылка сообщений от клиента 1 к клиенту 2
    while (1) {
        memset(buff, 0, BUFF_SIZE);

        // Чтение сообщения от клиента 1
        ssize_t read_size = read(client_1_socket, buff, BUFF_SIZE);
        if (read_size < 0) {
            error("read from client_1");
        }

        printf("Client_1: %s", buff);

        // Пересылка сообщения клиенту 2
        ssize_t write_size = write(client_2_socket, buff, strlen(buff));
        if (write_size < 0)
            error("write to client_2");

        // Проверка на завершение работы
        if (strcmp(buff, "The End\n") == 0)
            break;
    }

    // Закрытие соединений и сокетов
    close(client_1_socket);
    close(client_2_socket);
    close(server_s_fd);
    return 0;
}
