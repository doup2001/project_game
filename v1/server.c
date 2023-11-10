#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 12345
#define MAX_PLAYERS 10

struct Player {
    char name[20];
    int attempts;
};

struct Player players[MAX_PLAYERS];
int numPlayers = 0;

void startGame(int client_socket);
void displayRecords(int client_socket);
void SearchMyRecord(int client_socket);
void closeConnection(int server_socket, int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    // 서버 소켓 생성
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Server socket creation error");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 서버 소켓 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Server socket binding error");
        close(server_socket);
        exit(1);
    }

    // 클라이언트 대기 상태로 변경
    if (listen(server_socket, 5) == -1) {
        perror("Server listen error");
        close(server_socket);
        exit(1);
    }

    printf("서버 대기 중...\n");

    // 클라이언트 접속 대기
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size)) == -1) {
        perror("Server accept error");
        close(server_socket);
        exit(1);
    }

    printf("클라이언트 연결됨\n");

    int choice;

    while (1) {
        // 클라이언트가 선택한 메뉴를 받아옴
        recv(client_socket, &choice, sizeof(int), 0);

        switch (choice) {
            case 1:
                startGame(client_socket);
                break;
            case 2:
                displayRecords(client_socket);
                break;
            case 3:
                SearchMyRecord(client_socket);
                break;
            case 4:
                closeConnection(server_socket, client_socket);
                break;
            default:
                printf("올바른 선택이 아닙니다. 다시 선택하세요.\n");
                break;
        }
    }

    return 0;
}

void startGame(int client_socket) {
    // 클라이언트에게 랜덤 값을 전송
    int randValues[3];
    randValues[0] = rand() % 10;
    randValues[1] = rand() % 10;
    randValues[2] = rand() % 10;

    send(client_socket, randValues, sizeof(randValues), 0);

    // 게임 로직 구현
    int attempts;

    for (attempts = 1; attempts <= 10; attempts++) {
        int strike, ball;
        recv(client_socket, &strike, sizeof(int), 0);
        recv(client_socket, &ball, sizeof(int), 0);

        printf("%d 번째 시도 - 값 입력 (또는 'exit' 입력하여 종료): ", attempts);

        // 사용자 입력 받기
        int u1, u2, u3;
        scanf("%1d %1d %1d", &u1, &u2, &u3);

        // 결과 전송
        send(client_socket, &u1, sizeof(int), 0);
        send(client_socket, &u2, sizeof(int), 0);
        send(client_socket, &u3, sizeof(int), 0);

        printf(">>> %dS %dB\n", strike, ball);

        if (strike == 3) {
            printf("축하합니다! 정답을 맞추셨습니다.\n");
            break;
        }
    }

    if (attempts > 10) {
        printf("10번 시도 동안 정답을 맞추지 못했습니다. 정답은 %d %d %d 입니다.\n", randValues[0], randValues[1], randValues[2]);
    }

    // 게임이 끝나면 시도 횟수를 전송
    send(client_socket, &attempts, sizeof(int), 0);
}

void displayRecords(int client_socket) {
    // 서버에서 플레이어 기록 받음
    struct Player players[MAX_PLAYERS];
    recv(client_socket, players, sizeof(players), 0);

    // BEST 플레이어와 평균 시도 횟수 받음
    int minAttempts;
    char minAttemptsPlayer[20];

    recv(client_socket, &minAttempts, sizeof(int), 0);
    recv(client_socket, minAttemptsPlayer, sizeof(minAttemptsPlayer), 0);

    printf("BEST 플레이어 / 횟수 : %s / %d\n", minAttemptsPlayer, minAttempts);
    printf("--------------------\n");

    float averageAttempts;
    recv(client_socket, &averageAttempts, sizeof(float), 0);

    printf("평균 시도 횟수: %.2f\n", averageAttempts);
}

void SearchMyRecord(int client_socket) {
    // 클라이언트에게 찾을 플레이어 이름 요청
    char searchName[20];
    recv(client_socket, searchName, sizeof(searchName), 0);

    int found = 0;

    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, searchName) == 0) {
            // 찾은 플레이어 정보 전송
            send(client_socket, &players[i], sizeof(struct Player), 0);
            found = 1;
            break;
        }
    }

    if (!found) {
        // 플레이어를 찾지 못한 경우
        struct Player notFoundPlayer;
        strcpy(notFoundPlayer.name, "Not Found");
        send(client_socket, &notFoundPlayer, sizeof(struct Player), 0);
    }
}

void closeConnection(int server_socket, int client_socket) {
    // 클라이언트와의 연결 종료
    close(client_socket);

    // 서버 소켓 종료
    close(server_socket);

    printf("프로그램을 종료합니다.\n");
    exit(0);
}
