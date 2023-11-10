#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    while (1) {
        int choice;
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
    // 클라이언트 게임 시작 메세지 전달
    recv(client_socket, players[numPlayers].name, sizeof(players[numPlayers].name), 0);
    char startMessage[100];

    snprintf(startMessage, sizeof(startMessage), "안녕하세요, %s님! 게임을 시작합니다.", players[numPlayers].name);
    send(client_socket, &startMessage, sizeof(startMessage), 0);

    printf("[Client] %s\n", startMessage);

    // 클라이언트에게 랜덤 값을 전송
    int randValues[3];
    randValues[0] = rand() % 10;
    randValues[1] = rand() % 10;
    randValues[2] = rand() % 10;

    // 서버에 값 전달
    send(client_socket, randValues, sizeof(randValues), 0);

    // 게임 로직 구현
    // 게임 로직 구현
int attempts;

for (attempts = 1; attempts < 10; attempts++) {
    int strike = 0;
    int ball = 0;

    // 사용자 입력 받기
    int u1, u2, u3;
    recv(client_socket, &u1, sizeof(int), 0);
    recv(client_socket, &u2, sizeof(int), 0);
    recv(client_socket, &u3, sizeof(int), 0);

    // 서버에 기록 남기기
    printf("[Client] %d 번째 시도 - 값 %d %d %d \n", attempts, u1, u2, u3);

    // 과정 계산
    strike += (randValues[0] == u1) + (randValues[1] == u2) + (randValues[2] == u3);
    ball += (randValues[0] == u2) + (randValues[0] == u3) + (randValues[1] == u1) + (randValues[1] == u3) + (randValues[2] == u1) + (randValues[2] == u2);

    // 서버에 결과과정 저장
    printf("[Client] %d번째 시도 - %dS %dB\n", attempts, strike, ball);

    char message[100];
    
    if (strike == 3 && ball == 0) {
    snprintf(message, sizeof(message), "축하합니다! 정답을 맞추셨습니다.");

    printf("[Client] %s\n", message);  // 추가된 부분

    send(client_socket, &message, sizeof(char), 0);

    players[numPlayers].attempts = attempts;
    numPlayers++;

    break;
    }else if (strike !=3 && attempts <10)
    {
        snprintf(message, sizeof(message), "%d번째 시도 - %dS %dB\n", attempts, strike, ball);
        send(client_socket, message, sizeof(message), 0);

        

    } else{
        snprintf(message, sizeof(message), "10번 시도 동안 정답을 맞추지 못했습니다. 정답은 %d %d %d 입니다.\n", randValues[0], randValues[1], randValues[2]);
        
        send(client_socket, message, sizeof(message), 0);

        players[numPlayers].attempts = attempts;
        numPlayers++;

        break;
    }   
}


}


void displayRecords(int client_socket) {
    // 서버에서 플레이어 기록 받음

    int minAttempts = players[0].attempts;
    int totalAttempts = 0;
    char minAttemptsPlayer[20];  // 최소 시도를 기록한 플레이어 아이디 저장

    strcpy(minAttemptsPlayer, players[0].name);

    for (int i = 0; i < numPlayers; i++) {
        // printf("이름: %s, 시도 횟수: %d\n", players[i].name, players[i].attempts);

        // 최소 시도 횟수와 플레이어 아이디를 업데이트합니다.
        if (players[i].attempts < minAttempts) {
            minAttempts = players[i].attempts;
            strcpy(minAttemptsPlayer, players[i].name);
        }

        // 총 시도 횟수를 누적합니다.
        totalAttempts += players[i].attempts;
    }

    // BEST 플레이어와 평균 시도 횟수 받음
   
    send(client_socket, &minAttempts, sizeof(int), 0);
    send(client_socket, minAttemptsPlayer, sizeof(minAttemptsPlayer), 0);

    printf("BEST 플레이어 / 횟수 : %s / %d\n", minAttemptsPlayer, minAttempts);
    printf("--------------------\n");

    float averageAttempts;
    send(client_socket, &averageAttempts, sizeof(float), 0);

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
