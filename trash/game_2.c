#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // time 헤더 추가

#define MAX_PLAYERS 10  // 최대 플레이어 수
#define MAX_ATTEMPTS 10  // 최대 시도 횟수

struct Player {
    char name[20];
    int attempts;
};

struct Player players[MAX_PLAYERS];  // 플레이어 정보 배열
int numPlayers = 0;  // 현재까지의 플레이어 수

int randArr[3];  // 전역 변수로 변경
double averageAttempts;  // 전역 변수로 변경
int strike;  // 전역 변수로 변경

// 메뉴 항목을 출력하는 함수
void displayMenu();
void startGame();
void displayRecords();
void displayPlayerStats(char wantedName[20]);
void close();

int main() {
    int choice;
    char wantedName[20];

    srand((unsigned int)time(NULL));  // 시드 설정

    while (1) {
        system("clear");  // 화면 지우기
        displayMenu(); // 메뉴 항목 출력
        printf("값을 입력하세요: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                printf("이름을 입력하세요: ");
                scanf("%s", wantedName);
                displayPlayerStats(wantedName);
                break;
            case 3:
                displayRecords();
                break;
            case 4:
                close();
                break;
            default:
                printf("올바른 선택이 아닙니다. 다시 선택하세요.\n");
                break;
        }
    }

    return 0;
}

void displayMenu() {
    printf("메뉴를 선택하세요:\n");
    printf("1. Game Start\n");
    printf("2. Display Player Stats\n");
    printf("3. Display Records\n");
    printf("4. Close\n");
}

void startGame() {
    system("clear");  // 화면 지우기
    printf("게임을 시작합니다\n");

    if (numPlayers == MAX_PLAYERS) {
        printf("플레이어 기록이 가득 찼습니다. 더 이상 추가할 수 없습니다.\n");
        return;
    }

    printf("사용할 아이디를 입력하세요 : ");
    scanf("%s", players[numPlayers].name);

    char input[20];   // 최대 20글자의 입력을 가정합니다.

    for (int attempts = 1; attempts <= MAX_ATTEMPTS; ) {
        int u1, u2, u3; // 사용자 input값 입력
        int ball; // ball 값 초기화

        printf("%d 번째 시도 - 값 입력 (또는 'exit' 입력하여 종료): ", attempts);
        scanf("%s", input);

        // 사용자가 종료를 원하는지 확인
        if (strcmp(input, "exit") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        // 입력값을 정수로 변환
        sscanf(input, "%1d %1d %1d", &u1, &u2, &u3);

        // 스트라이크와 볼 계산
        randArr[0] = rand() % 10;
        randArr[1] = rand() % 10;
        randArr[2] = rand() % 10;
        strike = ball = 0;

        for (int i = 0; i < 3; i++) {
            strike += (randArr[i] == u1) + (randArr[i] == u2) + (randArr[i] == u3);
            ball += (randArr[i] == u2) + (randArr[i] == u3) + (randArr[(i + 1) % 3] == u1) + (randArr[(i + 1) % 3] == u3) + (randArr[(i + 2) % 3] == u1) + (randArr[(i + 2) % 3] == u2);
        }

        printf(">>> %dS %dB\n", strike, ball);

        if (strike == 3) {
            printf("축하합니다! 정답을 맞추셨습니다.\n");
            players[numPlayers].attempts = attempts;
            numPlayers++;  // 새 플레이어 추가
            break;
        }

        attempts++;
    }

    if (strike != 3) {
        printf("10번 시도 동안 정답을 맞추지 못했습니다. 정답은 %d %d %d 입니다.\n", randArr[0], randArr[1], randArr[2]);
        players[numPlayers].attempts = MAX_ATTEMPTS;
        numPlayers++;  // 새 플레이어 추가
    }
}

void close() {
    system("clear");  // 화면 지우기
    printf("프로그램을 종료합니다.\n");
    exit(0); // 수정: 0으로 변경
}

void displayRecords() {
    system("clear");  // 화면 지우기
    printf("플레이어 기록:\n");

    if (numPlayers == 0) {
        printf("기록이 없습니다.\n");
        return;
    }

    // 최소값, 평균 초기화
    int minAttempts = players[0].attempts;
    int totalAttempts = players[0].attempts;

    for (int i = 1; i < numPlayers; i++) {
        printf("이름: %s, 시도 횟수: %d\n", players[i].name, players[i].attempts);

        // 최소값 찾기
        if (players[i].attempts < minAttempts) {
            minAttempts = players[i].attempts;
        }

        // 평균 계산
        totalAttempts += players[i].attempts;
    }

    // 평균 계산
    averageAttempts = (double)totalAttempts / numPlayers;

    // 최소값, 평균 출력
    printf("최소 시도 횟수: %d\n", minAttempts);
    printf("시도 횟수 평균: %.2f\n", averageAttempts);
}

void displayPlayerStats(char wantedName[20]) {
    system("clear");  // 화면 지우기

    int playerIndex = -1;

    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, wantedName) == 0) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex == -1) {
        printf("해당 이름의 플레이어가 없습니다.\n");
        return;
    }

    printf("%s 플레이어의 통계:\n", wantedName);
    printf("시도 횟수: %d\n", players[playerIndex].attempts);

    // 평균 대비 차이 계산
    double difference = players[playerIndex].attempts - averageAttempts;

    printf("평균 대비 차이: %.2f%%\n", (difference / averageAttempts) * 100);
}
