#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_PLAYERS 10

struct Player {
    char name[20];
    int attempts;
};

struct Player players[MAX_PLAYERS];  // 플레이어 정보 배열
int numPlayers = 0;  // 현재까지의 플레이어 수

void displayMenu();
void startGame_cl(int server_socket);
void displayRecords_cl(int server_socket);
void SearchMyRecord_cl(int server_socket);

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    // 클라이언트 소켓 생성
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Client socket creation error");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 서버에 연결
    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Client connect error");
        close(server_socket);
        exit(1);
    }

    int choice;

    while (1) {
        displayMenu();
        printf("값을 입력하세요: ");
        scanf("%d", &choice);

        // 서버에 선택한 메뉴 전송
        send(server_socket, &choice, sizeof(int), 0);

        switch (choice) {
            case 1:
                startGame_cl(server_socket);
                break;
            case 2:
                displayRecords_cl(server_socket);
                break;
            case 3:
                SearchMyRecord_cl(server_socket);
                break;
            case 4:
                close(server_socket);
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

void startGame_cl(int server_socket) {
    system("clear");

    // 닉네임 입력
    printf("사용할 아이디를 입력하세요 : ");
    scanf("%s", players[numPlayers].name);
    send(server_socket, players[numPlayers].name, sizeof(players[numPlayers].name), 0);

    // 게임 시작 메세지
    char startMessage[100];
    recv(server_socket, &startMessage, sizeof(startMessage), 0);
    printf("[Server] %s\n", startMessage);

    // 가득 찬 경우 시작 X
    if (numPlayers == MAX_PLAYERS) {
        printf("플레이어 기록이 가득 찼습니다. 더 이상 추가할 수 없습니다.\n");
        return;
    }

    // 서버에서 랜덤 값을 받음
    int randValues[3];
    recv(server_socket, &randValues, sizeof(randValues), 0);

    int attempts;

    for (attempts = 1; attempts <= 10; attempts++) {
        printf("%d 번째 시도 - 값 입력 (또는 'exit' 입력하여 종료): ", attempts);

        // 사용자 입력 받기
        int u1, u2, u3;
        scanf("%1d %1d %1d", &u1, &u2, &u3);

        // 입력 값 전송
        send(server_socket, &u1, sizeof(int), 0);
        send(server_socket, &u2, sizeof(int), 0);
        send(server_socket, &u3, sizeof(int), 0);

        // 서버에서 결과 받기
        char message[100];
        recv(server_socket, &message, sizeof(message), 0);
        printf("[Server] %s\n", message);
       
    }
}






void displayRecords_cl(int server_socket) {
    // 서버에서 플레이어 기록 받음
    struct Player players[MAX_PLAYERS];
    recv(server_socket, players, sizeof(players), 0);

    // BEST 플레이어와 평균 시도 횟수 받음
    int receivedMinAttempts;
    char receivedMinAttemptsPlayer[20];

    recv(server_socket, &receivedMinAttempts, sizeof(int), 0);
    recv(server_socket, &receivedMinAttemptsPlayer, sizeof(receivedMinAttemptsPlayer), 0);

    printf("BEST 플레이어 / 횟수 : %s / %d\n", receivedMinAttemptsPlayer, receivedMinAttempts);
    printf("--------------------\n");

    float receivedAverageAttempts;
    recv(server_socket, &receivedAverageAttempts, sizeof(float), 0);

    printf("평균 시도 횟수: %.2f\n", receivedAverageAttempts);

}

void SearchMyRecord_cl(int server_socket) {
    // 사용자에게 찾을 플레이어 이름 입력 받음
    char searchName[20];
    printf("찾고 싶은 플레이어의 이름을 입력하세요: ");
    scanf("%s", searchName);

    // 입력 받은 이름을 서버에 전송
    send(server_socket, searchName, sizeof(searchName), 0);

    // 서버에서 플레이어 정보 받음
    struct Player searchResult;
    recv(server_socket, &searchResult, sizeof(struct Player), 0);

    if (strcmp(searchResult.name, "Not Found") == 0) {
        printf("찾는 플레이어가 없습니다.\n");
    } else {
        printf("플레이어: %s, 시도 횟수: %d\n", searchResult.name, searchResult.attempts);
    }
}
