#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
    WSADATA wsaData;
    int iResult;

    // Initialize WinSock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Create a UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address (localhost:8001)
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8001);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // --- Prepare message1: 5 int ---
    uint32_t msg1_values[5] = {1, 2, 3, 4, 5};
    char message1b[20];
    for (int i = 0; i < 5; i++) {
        uint32_t val = htonl(msg1_values[i]);
        memcpy(message1b + i * sizeof(uint32_t), &val, sizeof(uint32_t));
    }
    int message1Size = sizeof(message1b);

    // Send message1
    printf("Sending message1 (size %d bytes)\n", message1Size);
    int sent = sendto(sock, message1b, message1Size, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (sent == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive response
    char recvbuf[20];
    int addrLen = sizeof(serverAddr);
    printf("Waiting to receive...\n");
    int recvLen = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&serverAddr, &addrLen);
    if (recvLen == SOCKET_ERROR) {
        printf("recvfrom failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Received %d bytes\n", recvLen);

    // --- Prepare message2: 2 long long ---
    uint64_t msg2_values[2] = {10000000000ULL, 20000000000ULL};
    char message2b[16];
    for (int i = 0; i < 2; i++) {
        uint64_t val = htonll(msg2_values[i]);
        memcpy(message2b + i * sizeof(uint64_t), &val, sizeof(uint64_t));
    }
    int message2Size = sizeof(message2b);

    // Send message2
    printf("Sending message2 (size %d bytes)\n", message2Size);
    sent = sendto(sock, message2b, message2Size, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (sent == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive response for message2
    printf("Waiting to receive...\n");
    recvLen = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&serverAddr, &addrLen);
    if (recvLen == SOCKET_ERROR) {
        printf("recvfrom failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Received %d bytes\n", recvLen);

    // Clean up
    printf("Closing socket\n");
    closesocket(sock);
    WSACleanup();

    return 0;
}
