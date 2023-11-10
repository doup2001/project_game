#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_PLAYERS 20

struct Player {
    char name[20];
    int attempts;
};

void displayMenu();
void startGame(int client_socket);
void displayRecords(int client_socket);
void SearchMyRecord(int client_socket);

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // 클라이언트 소켓 생성
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Client socket creation error");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Client connect error");
        close(client_socket);
        exit(1);
    }

    int choice;

    while (1) {
        displayMenu();
        printf("값을 입력하세요: ");
        scanf("%d", &choice);

        // 서버에 선택한 메뉴 전송
        send(client_socket, &choice, sizeof(int), 0);

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
                close(client_socket);
                exit(0);
            default:
                printf("올바른 선택이 아닙니다. 다시 선택하세요.\n");
                break;
        }
    }

    return 0;
}

void displayMenu() {
    printf("\n");
    printf("메뉴를 선택하세요:\n");
    printf("1. 게임 시작\n");
    printf("2. 최고값 / 평균값\n");
    printf("3. 나의 기록 검색\n");
    printf("4. 게임 종료\n");
}

void startGame(int client_socket) {
    // 서버에서 랜덤 값을 받음
    int randValues[3];
    recv(client_socket, randValues, sizeof(randValues), 0);

    int attempts;

    for (attempts = 1; attempts <= 10; attempts++) {
        printf("%d 번째 시도 - 값 입력 (또는 'exit' 입력하여 종료): ", attempts);

        // 사용자 입력 받기
        int u1, u2, u3;
        scanf("%1d %1d %1d", &u1, &u2, &u3);

        // 입력 값 전송
        send(client_socket, &u1, sizeof(int), 0);
        send(client_socket, &u2, sizeof(int), 0);
        send(client_socket, &u3, sizeof(int), 0);

        // 서버에서 결과 받기
        int strike, ball;
        recv(client_socket, &strike, sizeof(int), 0);
        recv(client_socket, &ball, sizeof(int), 0);

        printf(">>> %dS %dB\n", strike, ball);

        if (strike == 3) {
            printf("축하합니다! 정답을 맞추셨습니다.\n");
            break;
        }
    }

    // 게임이 끝나면 서버에서 시도 횟수 받기
    recv(client_socket, &attempts, sizeof(int), 0);

    if (attempts > 10) {
        printf("10번 시도 동안 정답을 맞추지 못했습니다. 정답은 %d %d %d 입니다.\n", randValues[0], randValues[1], randValues[2]);
    }
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
    // 사용자에게 찾을 플레이어 이름 입력 받음
    char searchName[20];
    printf("찾고 싶은 플레이어의 이름을 입력하세요: ");
    scanf("%s", searchName);

    // 입력 받은 이름을 서버에 전송
    send(client_socket, searchName, sizeof(searchName), 0);

    // 서버에서 플레이어 정보 받음
    struct Player searchResult;
    recv(client_socket, &searchResult, sizeof(struct Player), 0);

    if (strcmp(searchResult.name, "Not Found") == 0) {
        printf("찾는 플레이어가 없습니다.\n");
    } else {
        printf("플레이어: %s, 시도 횟수: %d\n", searchResult.name, searchResult.attempts);
    }
}
