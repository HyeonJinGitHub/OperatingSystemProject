#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100

struct str_process
{
	int processID; //���μ��� ID
	int arriveTime; // �����ð�
	int serviceTime; // ���񽺽ð�
	int flag; // �غ�ť�� ���� ǥ�� (1�̸� �غ�ť 0�̸� ť�� ����)
	int waitTime; // ���ð�
};
int main()
{
	FILE* fp;
	int number;
	int nCount = 0;
	struct str_process process[MAX_PROCESS], temp_process[MAX_PROCESS];
	int quantum = 1; // q = 1
	int count = 0; // ���μ��� ����
	int time = 0; // ��ü ��ȯ�ð�
	int max_wait_index = 0, interrupt = 1, temp_time = 0,exit_process = 0;
	// ���� ���� ��ٸ� ���μ��� �ε��� , ���ͷ�Ʈ(���μ��� ����) �߻����� (1�̸� ���ͷ�Ʈ �߻�, �� �غ�ť���� ���� )
	//�� ���μ����� ��ȯ�ð�, ����� ���μ��� ����

	memset(process, 0, sizeof(process)); //0���� �ʱ�ȭ
	if ((fp = fopen("input.txt", "r")) == NULL) {
		fprintf(stderr, "������ �� �� �����ϴ�.");
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
		temp_process[i] = process[i]; //�ӽ� ���μ���
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("���μ��� ID\t �����ð�\t ���񽺽ð�\t ����ð� \t ��ȯ�ð�\t ����ȭ�� ��ȯ�ð�\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while (1) {
		for (int i = 0; i < count; i++) {
			if (process[i].arriveTime == time) { //i��° ���μ����� �����ϸ�, �� �غ�ť�� �����ԵǸ� 
				process[i].flag = 1; // �ش� ���μ����� ť�� ����
			}
			if (process[i].flag == 1 && interrupt == 1) { // i��° ���μ����� �غ�ť�� �ְ� ���ͷ�Ʈ�� 1�̸�
				if (process[i].serviceTime != 0 && process[i].waitTime >= process[max_wait_index].waitTime) {
					//i��° ���μ����� ���񽺽ð��� 0�� �ƴϰ�, 
					//i��° ���μ����� ���ð��� ���ݱ��� ���� ���� ��ٸ� ���μ����� ���ð����� �� ũ�ٸ�
					max_wait_index = i; //���� ������ٸ� ���μ����� i�� ��ü
				}
			}
		}
		//�� for�� ���������� �Ǹ� ���� ���� ��ٸ� ���μ������� ����
		process[max_wait_index].waitTime = 0; //���� ������ٸ� ���μ����� ���ð��� 0���� ����
		process[max_wait_index].serviceTime--; //���� ������ٸ� ���μ����� ���񽺽ð��� 1 ���� 
		temp_time++;//���μ��� ��ȯ �ð� 1 ����

		for (int i = 0; i < count; i++) {//ó������ ������ ��� ���μ���
			if (i != max_wait_index && process[i].flag == 1)//i��° ���μ����� ���� ���� ��ٸ� ���μ����� �ƴѵ�
				//i��° ���μ����� �غ�ť���� ������̸�
				process[i].waitTime++;//���ð� 1 ����
		}
		if (process[max_wait_index].serviceTime != 0 && temp_time != quantum) { // ���� ���� ��ٸ� ���μ����� ���񽺽ð��� 0�� �ƴϰ�
			//q ��ŭ �������� �ʾ�����
			interrupt = 0; //���ͷ�Ʈ �߻� x
		}
		else {
			if (process[max_wait_index].serviceTime == 0) {//���� ���� ��ٸ� ���μ����� ���񽺽ð���ŭ �����Ű��
				process[max_wait_index].flag = 0;//���� ���� ��ٸ� ���μ��� �غ�ť���� ����
				printf("%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %.2f \n", temp_process[max_wait_index].processID, temp_process[max_wait_index].arriveTime, temp_process[max_wait_index].serviceTime,
					time + 1, time + 1 - temp_process[max_wait_index].arriveTime, (time + 1 - temp_process[max_wait_index].arriveTime) / (double)temp_process[max_wait_index].serviceTime);
				//time+1�� ���� time�� ������Ű�� �ʾұ� ����
				exit_process++; //���� ���μ��� ���� ����
			}
			temp_time = 0;//���μ��� ��ȯ�ð� �ʱ�ȭ
			interrupt = 1;//���� ���μ��� ���ͷ�Ʈ �߻�
		}
		time++;//����ð� ����

		if (exit_process == count) break; //���� ���μ��� ������ ��ü ���μ��� ������ ������ break
	}
	system("pause");
	return 0;
}