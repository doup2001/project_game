#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 512

void err_quit(const char* msg) {
    perror(msg);
    exit(-1);
}

int main(int argc, char* argv[]) {
    // Initialize the WSA on Windows (no-op on macOS)
    // WSADATA wsa;
    // if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    //     return -1;

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        err_quit("socket()");
    }
    printf("소켓이 생성되었습니다.\n");

    // Server address information
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(9000);

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        err_quit("connect()");
    }
    printf("connect 연결요청!!\n");

    // Message to send to the server
    char buf[BUFSIZE + 1];
    sprintf(buf, "hello world!");

    // Send the message
    send(sock, buf, strlen(buf), 0);
    printf("메세지를 보냅니다.\n");

    // Close the socket
    close(sock);

    // Cleanup (no-op on macOS)
    // WSACleanup();

    return 0;
}
