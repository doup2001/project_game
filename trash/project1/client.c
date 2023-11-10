#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int client_socket;
    char buffer[1024];

    // 클라이언트 소켓 생성
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address) == -1)) {
        perror("connect");
        exit(1);
    }

    while (1) {
        // 클라이언트에서 메시지 입력
        printf("클라이언트: ");
        fgets(buffer, sizeof(buffer), stdin);

        // 메시지 전송
        send(client_socket, buffer, strlen(buffer), 0);

        // 서버로부터 메시지 수신
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("서버: %s\n", buffer);
    }

    // 소켓 닫기
    close(client_socket);
    return 0;
}
