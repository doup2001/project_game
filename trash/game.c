#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	int u1, u2, u3;
	int rand_1, rand_2, rand_3;
	int strike = 0, ball = 0;  // 초기화
	
	srand((unsigned)time(NULL)); // 랜덤 숫자 생성
	
	do{
		rand_1 = rand() % 10;
		rand_2 = rand() % 10;
		rand_3 = rand() % 10;
	}while(rand_1 == rand_2 || rand_2 == rand_3 || rand_1 == rand_3);
	
	printf("Computer : %d %d %d\n", rand_1, rand_2, rand_3);
	
	int attempts = 0;

do {
    strike = ball = 0;
    
    printf("Input : ");
    scanf("%d%d%d", &u1, &u2, &u3); // 3개의 숫자를 입력 받음
    
    // 랜덤의 숫자와 입력한 숫자가 일치하면 스트라이크 증가 아니면 볼 증가
    if(rand_1 == u1) strike++;            
    else if(rand_1 == u2) ball++;
    else if(rand_1 == u3) ball++;
    
    if(rand_2 == u2) strike++;
    else if(rand_2 == u1) ball++;
    else if(rand_2 == u3) ball++;
    
    if(rand_3 == u3) strike++;
    else if(rand_3 == u1) ball++;
    else if(rand_3 == u2) ball++;
    
    printf("\n%dS %dB\n", strike, ball);

    attempts++;
} while (strike != 3 && attempts < 10);
