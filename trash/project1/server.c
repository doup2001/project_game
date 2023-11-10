#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // 서버 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("소켓 생성 실패");
        exit(1);
    }

    // 서버 주소와 포트 설정
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 서버 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("바인딩 실패");
        exit(1);
    }

    // 클라이언트 연결 대기
    if (listen(server_socket, 5) == -1) {
        perror("대기 실패");
        exit(1);
    }

    // 클라이언트 연결 수락
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("연결 수락 실패");
        exit(1);
    } else
    printf("연결 완료\n ");

    char message[1024];

    while (1) {
        // 클라이언트로부터 메시지 수신
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received == -1) {
            perror("데이터 수신 실패");
            exit(1);
        } else if (bytes_received == 0) {
            printf("클라이언트 연결 종료\n");
            break;
        }

        // 수신한 메시지 출력
        message[bytes_received] = '\0';
        printf("클라이언트: %s\n", message);

        // 메시지를 클라이언트로 전송
        printf("서버: ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);
    }

    // 소켓 닫기
    close(client_socket);
    close(server_socket);

    return 0;
}
