#ifndef  _PROCESS_H_
#define  _PROCESS_H_
#include <malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>  


#define PROCESS_NAME_LENGTH 20
#define SLICE_TIME 3
typedef enum {
	READY, FINISH, RUN
}PROCESS_STATE;

typedef struct cc {
	
};


typedef struct PCB_SliceTime {
	char name[PROCESS_NAME_LENGTH];
	int pid; 
	PCB_SliceTime *next;
	int access_time; //����ʱ��
	int take_cpu_time; //ռ��CPUʱ��
	int process_time;//�߳�ʱ��
	PROCESS_STATE state; //״̬
}PCB_SliceTime;

typedef struct PCB_Priority{
	char name[PROCESS_NAME_LENGTH];
	int pid;
	PCB_Priority *next;
	int access_time; //����ʱ��
	int priority; //���ȼ�
	int take_cpu_time; //ռ��CPUʱ��
	int process_time;//�߳�ʱ��
	PROCESS_STATE state;
}PCB_Priority;



typedef struct {
	PCB_SliceTime *run;
	PCB_SliceTime *ready_head;
	PCB_SliceTime *ready_tail;
	PCB_SliceTime *finish;
}PCBC_SliceTime;


typedef struct {
	PCB_Priority *run;
	PCB_Priority *ready_head;
	PCB_Priority *ready_tail;
	PCB_Priority *finish;
}PCBC_Priority;

void AddToPCBCByPriority(PCBC_Priority *pcbc, PCB_Priority *pcb) {
	pcb->next = NULL;
	if (pcb->priority < pcbc->ready_head->priority) {
		pcbc->ready_head->next = pcb->next;
		pcb->next = pcbc->ready_head;
		pcbc->ready_head = pcb;
	}
	else {
		PCB_Priority *temp = pcbc->ready_head;
		while (temp->next != NULL) {
			if (pcb->priority < temp->next->priority) {
				pcb->next = temp->next->next;
				temp->next = pcb;
				break;
			}
			else {
				temp = temp->next;
			}
		}
	}
}
void AddToPCBCByAccessTime(PCBC_SliceTime* pcbc, PCB_SliceTime* pcb) {
	pcb->next = NULL;
	if (pcb->access_time < pcbc->ready_head->access_time) {
		pcb->next = pcbc->ready_head;
		pcbc->ready_head = pcb;
	}
	else {
		PCB_SliceTime* temp = pcbc->ready_head;
		while (temp->next != NULL)
		{
			if (pcb->access_time < temp->next->access_time) {

				break;
			}
			else {
				temp = temp->next;
			}

		}
		pcb->next = temp->next;
		temp->next = pcb;
	}
}

void GeneratePCBC_N(PCBC_SliceTime * pcbc, int processNum) {
	PCB_SliceTime *pcb;
	pcbc->run = pcbc->ready_head = pcbc->ready_tail = pcbc->finish = NULL;
	for (int i = 0; i < processNum; i++) {
		pcb = (PCB_SliceTime*)malloc(sizeof(PCB_SliceTime));
		_itoa_s(i, pcb->name, 10);
		srand((unsigned)time(NULL));
		pcb->access_time = rand() % processNum;
		pcb->process_time = rand() % 25;
		pcb->state = READY;
		pcb->next = NULL;

		if (pcbc->ready_head == NULL)
		{
			pcbc->ready_head = pcbc->ready_tail = pcb;
		}
		else {
			AddToPCBCByAccessTime(pcbc, pcb);
		}
	}
}

void GeneratePCBC(PCBC_Priority * pcbc, int processNum) {
	PCB_Priority *pcb;
	pcbc->run = pcbc->ready_head = pcbc->ready_tail = pcbc->finish = NULL;
	for (int i = 0; i < processNum; i++) {
		pcb = (PCB_Priority*)malloc(sizeof(PCB_Priority));
		_itoa_s(i, pcb->name, 10);
		srand((unsigned)time(NULL));
		pcb->access_time = rand() % processNum;
		pcb->process_time = rand() % 25;
		pcb->state = READY;
		pcb->next = NULL;

		if (pcbc->ready_head == NULL)
		{
			pcbc->ready_head = pcbc->ready_tail = pcb;
		}
		else {
			AddToPCBCByPriority(pcbc, pcb);
		}
	}
}



void Printf_PCBC(PCBC_SliceTime *pcbc) {
	printf("run:%s\n ", pcbc->run->name);

	PCB_SliceTime *temp;
	temp = pcbc->ready_head;

	printf("ready:\n");
	while (temp != NULL) {
		printf("%s\n", temp->name);
	}

	temp = pcbc->finish;
	printf("finish:\n");
	while (temp != NULL) {
		printf("%s\n", temp->name);
	}
}
void Printf_PCBC(PCBC_Priority *pcbc)
{
	printf("run:%s\n ", pcbc->run->name);

	PCB_Priority *temp;
	temp = pcbc->ready_head;

	printf("ready:\n");
	while (temp != NULL) {
		printf("%s\n", temp->name);
	}

	temp = pcbc->finish;
	printf("finish:\n");
	while (temp != NULL) {
		printf("%s\n", temp->name);
	}
}



void Run_Process_SliceTime(PCBC_SliceTime* pcbc) {
	while (pcbc->ready_head != NULL) {
		//��ʼ�µ�һ��ʱ��Ƭ
		int slice_time = 0;
		//��ʼһ���µĽ���
		pcbc->run = pcbc->ready_head;
		pcbc->run->state = RUN;
		Printf_PCBC(pcbc);
		while (slice_time < SLICE_TIME) {
			pcbc->run->take_cpu_time++;
			//���̽���
			if (pcbc->run->take_cpu_time >= pcbc->run->process_time)
			{
				break;
			}
		}
		//ʱ��Ƭ����֮����ε���
		if (pcbc->run->take_cpu_time >= pcbc->run->process_time)
		{
			//ʱ��Ƭ����֮ǰ��ɣ�����ɵĽ����ƶ�����������
			printf("���̣�%s�������С�\n", pcbc->run->name);
			pcbc->run->state = FINISH;
			pcbc->run->next = pcbc->finish;
			pcbc->finish = pcbc->run;
			pcbc->ready_head = pcbc->ready_head->next;
		}
		else
		{
			//ʱ��Ƭ����֮�󣬽���δ��ɣ��ƶ�����������β��
			pcbc->run->state = READY;
			pcbc->ready_head = pcbc->ready_head->next;
			pcbc->run->next = NULL;
			pcbc->ready_tail = pcbc->run;
		}
	}
}
void Run_Process_Priority(PCBC_Priority *pcbc) {
	while (pcbc->ready_head != NULL) {
		//��ʼ�µ�һ��ʱ��Ƭ
		int slice_time = 0;
		//��ʼһ���µĽ���
		pcbc->run = pcbc->ready_head;
		pcbc->run->state = RUN;
		Printf_PCBC(pcbc);
		while (slice_time < SLICE_TIME) {
			pcbc->run->take_cpu_time++;
			//���̽���
			if (pcbc->run->take_cpu_time >= pcbc->run->process_time)
			{
				break;
			}
		}
		//ʱ��Ƭ����֮����ε���
		if (pcbc->run->take_cpu_time >= pcbc->run->process_time)
		{
			//ʱ��Ƭ����֮ǰ��ɣ�����ɵĽ����ƶ�����������
			printf("���̣�%s�������С�\n", pcbc->run->name);
			pcbc->run->state = FINISH;
			pcbc->run->next = pcbc->finish;
			pcbc->finish = pcbc->run;
			pcbc->ready_head = pcbc->ready_head->next;
		}
		else
		{
			//ʱ��Ƭ����֮�󣬽���δ���
			//�������ȼ���������
			pcbc->run->state = READY;
			pcbc->ready_head = pcbc->ready_head->next;
			pcbc->run->next = NULL;
			//�������ȼ���������
			AddToPCBCByPriority(pcbc, pcbc->run);
		}
	}
}


#endif