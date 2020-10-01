#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100

struct str_process
{
	int processID; //프로세스 ID
	int arriveTime; // 도착시간
	int serviceTime; // 서비스시간
	int flag; // 준비큐에 넣음 표시 (1이면 준비큐 0이면 큐에 없음)
	int waitTime; // 대기시간
};
int main()
{
	FILE* fp;
	int number;
	int nCount = 0;
	struct str_process process[MAX_PROCESS], temp_process[MAX_PROCESS];
	int quantum = 1; // q = 1
	int count = 0; // 프로세스 개수
	int time = 0; // 전체 반환시간
	int max_wait_index = 0, interrupt = 1, temp_time = 0,exit_process = 0;
	// 가장 오래 기다린 프로세스 인덱스 , 인터럽트(프로세스 실행) 발생여부 (1이면 인터럽트 발생, 즉 준비큐에서 실행 )
	//각 프로세스의 반환시간, 종료된 프로세스 개수

	memset(process, 0, sizeof(process)); //0으로 초기화
	if ((fp = fopen("input.txt", "r")) == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다.");
		exit(1);
	}
	while (!feof(fp)) {
			fscanf(fp, "%d,", &number);
			if (nCount == 0) {
				process[count].processID = number;
			}
			else if (nCount == 1) {
				process[count].arriveTime = number;
			}
			else {
				process[count].serviceTime = number;
			}
			if (process[count].processID == 0)
				break;
			if (nCount == 2) {
				nCount = -1;
				count++;
			}
			nCount++;
	}
	fclose(fp);
	for (int i = 0; i < count; i++) {
		temp_process[i] = process[i]; //임시 프로세스
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("프로세스 ID\t 도착시간\t 서비스시간\t 종료시간 \t 반환시간\t 정규화된 반환시간\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while (1) {
		for (int i = 0; i < count; i++) {
			if (process[i].arriveTime == time) { //i번째 프로세스가 도착하면, 즉 준비큐에 들어오게되면 
				process[i].flag = 1; // 해당 프로세스를 큐에 넣음
			}
			if (process[i].flag == 1 && interrupt == 1) { // i번째 프로세스가 준비큐에 있고 인터럽트도 1이면
				if (process[i].serviceTime != 0 && process[i].waitTime >= process[max_wait_index].waitTime) {
					//i번째 프로세스의 서비스시간이 0이 아니고, 
					//i번째 프로세스의 대기시간이 지금까지 가장 오래 기다린 프로세스의 대기시간보다 더 크다면
					max_wait_index = i; //가장 오래기다린 프로세스를 i로 교체
				}
			}
		}
		//위 for문 빠져나오게 되면 가장 오래 기다린 프로세스부터 실행
		process[max_wait_index].waitTime = 0; //가장 오래기다린 프로세스의 대기시간을 0으로 만듬
		process[max_wait_index].serviceTime--; //가장 오래기다린 프로세스의 서비스시간은 1 감소 
		temp_time++;//프로세스 반환 시간 1 증가

		for (int i = 0; i < count; i++) {//처음부터 끝까지 모든 프로세스
			if (i != max_wait_index && process[i].flag == 1)//i번째 프로세스가 가장 오래 기다린 프로세스가 아닌데
				//i번째 프로세스가 준비큐에서 대기중이면
				process[i].waitTime++;//대기시간 1 증가
		}
		if (process[max_wait_index].serviceTime != 0 && temp_time != quantum) { // 가장 오래 기다린 프로세스의 서비스시간이 0이 아니고
			//q 만큼 실행하지 않았으면
			interrupt = 0; //인터럽트 발생 x
		}
		else {
			if (process[max_wait_index].serviceTime == 0) {//가장 오래 기다린 프로세스를 서비스시간만큼 실행시키면
				process[max_wait_index].flag = 0;//가장 오래 기다린 프로세스 준비큐에서 빼냄
				printf("%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %.2f \n", temp_process[max_wait_index].processID, temp_process[max_wait_index].arriveTime, temp_process[max_wait_index].serviceTime,
					time + 1, time + 1 - temp_process[max_wait_index].arriveTime, (time + 1 - temp_process[max_wait_index].arriveTime) / (double)temp_process[max_wait_index].serviceTime);
				//time+1은 아직 time을 증가시키지 않았기 때문
				exit_process++; //종료 프로세스 개수 증가
			}
			temp_time = 0;//프로세스 반환시간 초기화
			interrupt = 1;//다음 프로세스 인터럽트 발생
		}
		time++;//종료시간 증가

		if (exit_process == count) break; //종료 프로세스 개수가 전체 프로세스 개수와 같으면 break
	}
	system("pause");
	return 0;
}