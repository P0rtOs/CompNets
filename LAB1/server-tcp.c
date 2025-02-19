#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8001

void receive_int_data(SOCKET client_sock, int *buffer, size_t size) {
    size_t total_size = size * sizeof(int);
    size_t bytes_received = 0;

    while (bytes_received < total_size) {
        int received = recv(client_sock, (char*)buffer + bytes_received, total_size - bytes_received, 0);
        if (received <= 0) {
            perror("recv failed");
            closesocket(client_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        bytes_received += received;
    }

    printf("Received int data:\n");
    for (size_t i = 0; i < size; i++) {
        buffer[i] = ntohl(buffer[i]);
        printf("%d ", buffer[i]);
    }
    printf("\n");

    send(client_sock, "OK", 2, 0);
}

void receive_long_data(SOCKET client_sock, long long *buffer, size_t size) {
    size_t total_size = size * sizeof(long long);
    size_t bytes_received = 0;

    while (bytes_received < total_size) {
        int received = recv(client_sock, (char*)buffer + bytes_received, total_size - bytes_received, 0);
        if (received <= 0) {
            perror("recv failed");
            closesocket(client_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        bytes_received += received;
    }

    printf("Received long data:\n");
    for (size_t i = 0; i < size; i++) {
        buffer[i] = ntohll(buffer[i]);
        printf("%lld ", buffer[i]);
    }
    printf("\n");

    send(client_sock, "OK", 2, 0);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Bind failed");
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 1) == SOCKET_ERROR) {
        perror("Listen failed");
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection on %s:%d...\n", SERVER_IP, SERVER_PORT);
    struct sockaddr_in client_addr;
    int client_size = sizeof(client_addr);
    SOCKET client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
    if (client_sock == INVALID_SOCKET) {
        perror("Accept failed");
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    int int_data[5];
    long long long_data[2];

    receive_int_data(client_sock, int_data, 5);
    receive_long_data(client_sock, long_data, 2);

    printf("Closing connection\n");
    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();
    return 0;
}
