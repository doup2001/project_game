#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 512

void err_quit(const char* msg) {
    perror(msg);
    exit(-1);
}

int main(int argc, char* argv[]) {
    int retval;

    // 소켓 초기화
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        err_quit("socket()");
    }
    printf("소켓이 생성되었습니다\n");

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(9000);

    if (bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        err_quit("bind()");
    }
    printf("Bind 완료 되었습니다.\n");

    if (listen(listen_sock, SOMAXCONN) == -1) {
        err_quit("listen()");
    }
    printf("connect 연결을 기다리는 중..\n");

    // 통신에 사용할 변수
    int client_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char buf[BUFSIZE + 1];

    // accept()
    addrlen = sizeof(clientaddr);
    client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
    printf("요청을 받았습니다.\n");

    // 받는 데이터 출력
    retval = recv(client_sock, buf, BUFSIZE, 0);
    buf[retval] = '\0';
    printf("[TCP/%s:%d] %s\n",
           inet_ntoa(clientaddr.sin_addr),
           ntohs(clientaddr.sin_port), buf);

    // closesocket
    close(listen_sock);

    return 0;
}
