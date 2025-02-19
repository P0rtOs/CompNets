#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8001);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Start on %s port %d\n", "localhost", 8001);

    while (1) {
        char recvBuf[20];
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        printf("Waiting to receive message\n");
        int recvLen = recvfrom(sock, recvBuf, sizeof(recvBuf), 0,
                               (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (recvLen == SOCKET_ERROR) {
            printf("recvfrom failed: %d\n", WSAGetLastError());
            break;
        }

        printf("Received %d bytes from %s:%d\n", recvLen,
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        if (recvLen > 0) {
            int sent = sendto(sock, recvBuf, recvLen, 0,
                              (struct sockaddr *)&clientAddr, clientAddrLen);
            if (sent == SOCKET_ERROR) {
                printf("sendto failed: %d\n", WSAGetLastError());
                break;
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
