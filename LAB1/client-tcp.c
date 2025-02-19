#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8001

void send_int_data(SOCKET sock, int *data, size_t size) {
    size_t bytes_sent = 0;
    size_t total_size = size * sizeof(int);
    char buffer[16];

    for (size_t i = 0; i < size; i++) {
        int value = htonl(data[i]);
        send(sock, (char*)&value, sizeof(int), 0);
    }
    
    printf("Sent int data:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    while (bytes_sent < total_size) {
        int received = recv(sock, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            perror("recv failed");
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        bytes_sent += received;
        printf("Received: %.*s\n", received, buffer);
    }
}

void send_long_data(SOCKET sock, long long *data, size_t size) {
    size_t bytes_sent = 0;
    size_t total_size = size * sizeof(long long);
    char buffer[16];

    for (size_t i = 0; i < size; i++) {
        long long value = htonll(data[i]);
        send(sock, (char*)&value, sizeof(long long), 0);
    }
    
    printf("Sent long data:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%lld ", data[i]);
    }
    printf("\n");

    while (bytes_sent < total_size) {
        int received = recv(sock, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            perror("recv failed");
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        bytes_sent += received;
        printf("Received: %.*s\n", received, buffer);
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("Connecting to %s port %d\n", SERVER_IP, SERVER_PORT);
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Connection failed");
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    int message1[] = {10, 20, 30, 40, 50};
    long long message2[] = {10000000000LL, 20000000000LL};

    send_int_data(sock, message1, sizeof(message1) / sizeof(int));
    send_long_data(sock, message2, sizeof(message2) / sizeof(long long));

    printf("Closing socket\n");
    closesocket(sock);
    WSACleanup();
    return 0;
}
