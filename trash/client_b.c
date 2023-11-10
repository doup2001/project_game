#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 10  // 최대 플레이어 수

struct Player {
    char name[20];
    int attempts;
};

struct Player players[MAX_PLAYERS];  // 플레이어 정보 배열
int numPlayers = 0;  // 현재까지의 플레이어 수

int u1, u2, u3; // 사용자 input값 입력
int rand_1, rand_2, rand_3; // 랜덤 값 부여
int strike, ball; // strike와 ball 값 초기화

// 메뉴 항목을 출력하는 함수
void displayMenu();
void startGame();
void close();
void displayRecords();

int main() {
    int choice;

    do { // 임의의 3가지 수 초기화
        rand_1 = rand() % 10;
        rand_2 = rand() % 10;
        rand_3 = rand() % 10;
    } while (rand_1 == rand_2 || rand_2 == rand_3 || rand_1 == rand_3);

    while (1) {
        displayMenu(); // 메뉴 항목 출력
        printf("값을 입력하세요: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
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
    printf("2. Display Records\n");
    printf("3. Close\n");
}

void startGame() {
    system("clear");
    printf("게임을 시작합니다\n");

    if (numPlayers == MAX_PLAYERS) {
        printf("플레이어 기록이 가득 찼습니다. 더 이상 추가할 수 없습니다.\n");
        return;
    }

    printf("사용할 아이디를 입력하세요 : ");
    scanf("%s", players[numPlayers].name);

    char input[20];   // 최대 20글자의 입력을 가정합니다.

    for (int attempts = 1; attempts <= 10; ) {
        strike = ball = 0;

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
        strike += (rand_1 == u1) + (rand_2 == u2) + (rand_3 == u3);
        ball += (rand_1 == u2) + (rand_1 == u3) + (rand_2 == u1) + (rand_2 == u3) + (rand_3 == u1) + (rand_3 == u2);

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
        printf("10번 시도 동안 정답을 맞추지 못했습니다. 정답은 %d %d %d 입니다.\n", rand_1, rand_2, rand_3);
        players[numPlayers].attempts = 10;
        numPlayers++;  // 새 플레이어 추가
    }
}

void close() {
    system("clear");
    printf("프로그램을 종료합니다.\n");
    exit(0); // 수정: 0으로 변경
}

void displayRecords() {
    system("clear");
    printf("플레이어 기록:\n");

    if (numPlayers == 0) {
        printf("기록이 없습니다.\n");
        return;
    }

    int minAttempts = players[0].attempts;
    int totalAttempts = players[0].attempts;
    char minAttemptsPlayer[20];  // Store the ID of the player with the minimum attempts

    // Copy the ID of the first player to start comparison
    strcpy(minAttemptsPlayer, players[0].name);

    for (int i = 0; i < numPlayers; i++) {
        
        // Update minimum attempts and player ID
        if (players[i].attempts < minAttempts) {
            minAttempts = players[i].attempts;
            strcpy(minAttemptsPlayer, players[i].name);
        }

        // Accumulate total attempts
        totalAttempts += players[i].attempts;
    }

    // Calculate average attempts
    float averageAttempts = (float)totalAttempts / numPlayers;

    printf("최소 시도 횟수를 기록한 플레이어: %s\n", minAttemptsPlayer);
    printf("최소 시도 횟수: %d\n", minAttempts);
    printf("평균 시도 횟수: %.2f\n", averageAttempts);
}
