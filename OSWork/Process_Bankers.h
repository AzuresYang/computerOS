#ifndef  _PROCESS_BANKERS_H_
#define  _PROCESS_BANKERS_H_
#include <malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>  
#include <assert.h>

#define PROCESS_NAME_LENGTH 20
#define SLICE_TIME 3

#define RESOURCE_MAX 30  //�����Դ��
#define RESOURCE_SIZE 3
#define PROCESS_NUM 5

int g_resource_max[PROCESS_NUM][RESOURCE_SIZE]; //ÿ�����̵������Դ����
int g_resource_allocation[PROCESS_NUM][RESOURCE_SIZE]; //ÿ�������Ѿ����䵽����Դ����
int g_resource_available[RESOURCE_SIZE];//ϵͳ������Դ����
int g_resource_need[PROCESS_NUM][RESOURCE_SIZE]; //ÿ������������Դ��

typedef enum {
	READY, FINISH, RUN,BLOCK
}PROCESS_STATE;

typedef struct PCB_SliceTime {
	char name[PROCESS_NAME_LENGTH];
	int pid;
	PCB_SliceTime *next;
	int resources[RESOURCE_SIZE];
	int access_time; //����ʱ��
	int take_cpu_time; //ռ��CPUʱ��
	int process_time;//�߳�ʱ��
	PROCESS_STATE state; //״̬
}PCB_SliceTime;

typedef struct {
	PCB_SliceTime *run;
	PCB_SliceTime *ready_head;
	PCB_SliceTime *ready_tail;
	PCB_SliceTime *block;
	PCB_SliceTime *finish;
}PCBC_SliceTime;

typedef struct{
	PCB_SliceTime *process;
	int need[RESOURCE_SIZE];
	int allcation[RESOURCE_SIZE];
	int request[RESOURCE_SIZE];
	bool isValid;
	bool hadDealRequest;
	int idx;
}ProcessWithResource;

typedef struct {
	ProcessWithResource processes[PROCESS_NUM];

	int resourceavailable[RESOURCE_SIZE];
	int resourceMax[RESOURCE_SIZE];
	int processNum;

}Process_Resource_Map;


static  ProcessWithResource* Get_PCBR_ByProcess(Process_Resource_Map *map, PCB_SliceTime* process) {
	for (int i = 0; i < PROCESS_NUM; i++)
		if (map->processes[i].process != NULL && map->processes[i].process == process)
			return &map->processes[i];
	printf("������Դ����ģ����û�иý���:%s",process->name);
	assert(false);
	return NULL;
}
static ProcessWithResource* Get_PCBR_ByIdx(Process_Resource_Map *map, int idx) {
	if (idx < 0 || idx >= PROCESS_NUM)
	{
		printf("��Դ�У������±����%d", idx);
		assert(false);
		return NULL;
	}
	for (int i = 0; i < PROCESS_NUM; i++)
		if (map->processes[i].process != NULL && map->processes[i].idx == idx)
			return &map->processes[i];

	printf("������Դ����ģ����û�и��±����:%d", idx);
	assert(false);
	return NULL;
}

static void AddToPCBCByAccessTime(PCBC_SliceTime* pcbc, PCB_SliceTime* pcb) {
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


void GeneratePCBC(PCBC_SliceTime * pcbc, int processNum) {
	
	PCB_SliceTime *pcb;
	pcbc->run = pcbc->ready_head = pcbc->ready_tail = pcbc->finish = pcbc->block=NULL;
	for (int i = 0; i < processNum; i++) {
		pcb = (PCB_SliceTime*)malloc(sizeof(PCB_SliceTime));
		_itoa_s(i, pcb->name, 10);
		srand((unsigned)time(NULL));
		pcb->access_time = rand() % processNum;
		pcb->process_time = rand() % 25;
		pcb->state = READY;
		pcb->next = NULL;

		//������Դ
		for (int i = 0; i < RESOURCE_SIZE; i++)
			pcb->resources[i] = (rand() % RESOURCE_MAX)/2;

		if (pcbc->ready_head == NULL)
		{
			pcbc->ready_head = pcb;
		}
		else {
			AddToPCBCByAccessTime(pcbc, pcb);
		}
	}
}

void PrintfResourceLine() {
	for (int i = 0; i < PROCESS_NUM; i++)
	{
		printf("%4c",i);
	}
}

void Printf_Max(Process_Resource_Map* map) {
	printf("\n");
	printf("\nMaxMaxtrix:\n");
	printf("%4s", &"pid");
	PrintfResourceLine();
	printf("\n");

	for (int i = 0; i < PROCESS_NUM; i++)
	{
		if (!map->processes[i].isValid)
			continue;
		PCB_SliceTime *pro = map->processes[i].process;
		printf("%4s", pro->name);
		char str[4];
		
		for (int j = 0; j < RESOURCE_SIZE; j++)
		{
			_itoa_s(map->processes[i].process->resources[j], str, 10);
			printf("%4s", str);
		}
		printf("\n");
	}
}
void Printf_Resource(Process_Resource_Map *map)
{
	printf("\n");
	printf("\nAllocationMaxtrix:\n");
	printf("%4s", &"pid");
	PrintfResourceLine();
	printf("processNum:");
	printf("\n");

	for (int i = 0; i < PROCESS_NUM; i++)
	{
		if(!map->processes[i].isValid)
			continue;
		PCB_SliceTime *pro = map->processes[i].process;
		printf("%4s", pro->name);
		char str[4];
		for (int j = 0; j < RESOURCE_SIZE; j++)
		{
			_itoa_s(map->processes[i].allcation[j], str, 10);
			printf("%4s",str);
		}

	}

	printf("\n");
	printf("\nNeed:\n");
	printf("%4s", &"pid");
	PrintfResourceLine();
	printf("\n");

	for (int i = 0; i < PROCESS_NUM; i++)
	{
		if (!map->processes[i].isValid)
			continue;
		PCB_SliceTime *pro = map->processes[i].process;
		printf("%4s", pro->name);
		char str[4];
		for (int j = 0; j < RESOURCE_SIZE; j++)
		{
			_itoa_s(map->processes[i].need[j], str, 10);
			printf("%4s", str);
		}

	}

	printf("\n");
	printf("\nAllocationMaxtrix:\n");
	printf("%4s", &"pid");
	PrintfResourceLine();
	printf("\n");

	for (int i = 0; i < PROCESS_NUM; i++)
	{
		if (!map->processes[i].isValid)
			continue;
		PCB_SliceTime *pro = map->processes[i].process;
		printf("%4s", pro->name);
		char str[4];
		for (int j = 0; j < RESOURCE_SIZE; j++)
		{
			_itoa_s(map->processes[i].allcation[j], str, 10);
			printf("%4s", str);
		}

	}

}
 void Printf_PCBC_N(PCBC_SliceTime *pcbc) {
	printf("\nrun:%s\n ", pcbc->run->name);

	PCB_SliceTime *temp;
	temp = pcbc->ready_head;
	printf("\n");
	printf("ready:\n");
	while (temp != NULL) {

		printf("%s\n", temp->name);
		temp = temp->next;
	}
	/*while (pcbc->ready_head != NULL) {

		printf("%s\n", pcbc->ready_head->name);
		pcbc->ready_head = pcbc->ready_head->next;
	}*/
	printf("\n");
	temp = pcbc->block;
	printf("block:\n");
	while (temp != NULL) {

		printf("%s\n", temp->name);
		temp = temp->next;
	}
	printf("\n");
	temp = pcbc->finish;
	printf("finish:\n");
	while (temp != NULL) {
		
		printf("%s\n", temp->name);
		temp = temp->next;
	}
	
}

static int FindSatisfiedCondition(ProcessWithResource **pcbrs, int *work, bool *finish, int processNum) {
	 for (int i = 0; i < processNum; i++) {
		 if (finish[i] == false)
		 {
			 bool isout = false;
			 for (int j = 0; j < RESOURCE_SIZE; j++)
			 {
				 //��Դ����
				 if (pcbrs[j]->need[j] > work[j])
					 isout = true;
			 }
			 if (!isout)
				 return i;
		 }
	 }
	 return -1;
 }

static bool Is_SystemSafe(Process_Resource_Map *map) {
	int work[RESOURCE_SIZE];
	for (int i = 0; i < RESOURCE_SIZE; i++)
		work[i] = map->resourceavailable[i];

	int processIdx = 0;
	bool *finish = (bool*)malloc(map->processNum*sizeof(bool));
	ProcessWithResource **pcbrs = (ProcessWithResource**)malloc(map->processNum*sizeof(ProcessWithResource *));
	
	//�ҵ������еĽ���
	for (int i = 0; i < PROCESS_NUM; i++)
	{
		if (map->processes[i].isValid) {
			finish[processIdx] = true;
			pcbrs[processIdx] = &map->processes[i];
			processIdx++;
		}			
	}
	int processNum = processIdx;
	bool result = true;
	for (int i = 0; i < processNum; i++) {
		int idx = FindSatisfiedCondition(pcbrs,work,finish, processNum);
		if (idx < 0)
		{
			result = false;
			break;
		}
		finish[idx] = true;
		for (int j = 0; j < RESOURCE_SIZE; j++)
		{
			work[j] += pcbrs[idx]->allcation[j]; //ȡ����Դ
		}
	}
	free(finish);
	free(pcbrs);
	return true;
}

static void Add_Process_To_Ready(PCBC_SliceTime* pcbc, PCB_SliceTime *pcb) {
	PCB_SliceTime *temp;
	pcb->next = NULL;
	if (pcbc->ready_head == NULL)
		pcbc->ready_head = pcb;
	else
	{
		temp = pcbc->ready_head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = pcb;
		pcb->next = NULL;
		pcb->state = READY;
	}
}
static void Add_Process_To_Block(PCBC_SliceTime* pcbc, PCB_SliceTime *pcb) {
	PCB_SliceTime *temp;
	pcb->next = NULL;
	if (pcbc->block == NULL)
		pcbc->block = pcb;
	else
	{
		temp = pcbc->block;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = pcb;
		pcb->state = BLOCK;
	}
}
static bool IsRunFinish(Process_Resource_Map *map, PCB_SliceTime *pcb)
{
	ProcessWithResource* pcbr = Get_PCBR_ByProcess(map,pcb);
	for (int i = 0; i < RESOURCE_SIZE; i++)
	{
		if (pcbr->need[i] > 0)
			return false;
	}
	return true;
}



static void Generate_Request(ProcessWithResource *pcbr) {
	if (pcbr->hadDealRequest)
	{
		printf("����һ������\n");
		for (int i = 0; i < RESOURCE_SIZE; i++)
		{
			int val = rand() % (pcbr->need[i] + 5);
			pcbr->request[i] = val > pcbr->need[i] ? pcbr->need[i] : val;
			printf("����[%s]������Դ[%d]����[%d]---Ŀǰ������Դ����[%d]\n", pcbr->process->name, i, pcbr->request[i], pcbr->need[i]);
		}
		pcbr->hadDealRequest = false;
	}
}
static bool Try_AllcateRequestResource_SystemIsSafe(Process_Resource_Map *map, ProcessWithResource* pcbr) {
	printf("����[%s]������Դ����[%d,%d,%d]\n", pcbr->process->name, pcbr->need[0], pcbr->need[1], pcbr->need[2]);
	printf("����[%s]������Դ����[%d,%d,%d]\n", pcbr->process->name, pcbr->request[0], pcbr->request[1], pcbr->request[2]);
	for (int i = 0; i < RESOURCE_SIZE; i++)
	{
		map->resourceavailable[i] -= pcbr->request[i];
		pcbr->allcation[i] += pcbr->request[i];
		if (pcbr->request[i] > pcbr->need[i])
		{
			printf("����[%s]������Դ[%d]����[%d]����������Դ����[%d]\n", pcbr->process->name, i, pcbr->request[i], pcbr->need[i]);
			return false;
		}
		else
			pcbr->need[i] -= pcbr->request[i];
	}
	//ϵͳ��Ȼ��ȫ
	if (Is_SystemSafe(map))
	{
		pcbr->hadDealRequest = true;
		
		//������ȫ
		printf("����ͨ��\n");
		printf("����[%s]Ŀǰ������Դ����[%d,%d,%d]\n", pcbr->process->name, pcbr->need[0], pcbr->need[1], pcbr->need[2]);
		return true;
	}
	else
	{
		//��ԭ
		for (int i = 0; i < RESOURCE_SIZE; i++)
		{
			map->resourceavailable[i] += pcbr->request[i];
			pcbr->allcation[i] -= pcbr->request[i];
			pcbr->need[i] += pcbr->request[i];
		}
		printf("����ͨ��\n");
		return false;
	}

}

//��Map�����һ��Process
static void Replace_Process_To_ResourceMap(Process_Resource_Map *map,PCB_SliceTime *pcb, int idx) {
	map->processes[idx].process = pcb;
	map->processes[idx].idx = map->processNum;
	map->processes[idx].hadDealRequest = true;
	map->processes[idx].isValid = true;

	//��ʼ����Դ
	for (int i = 0; i < RESOURCE_SIZE; i++) {
		map->processes[idx].allcation[i] = 0;
		map->processes[idx].need[i] = pcb->resources[i]; //��Դ��ʼ��
	
	}
}
static void Delete_ProcessResource_Frome_ResourceMap(Process_Resource_Map *map, PCB_SliceTime *pcb, int idx) {
	map->processes[idx].process = NULL;
	map->processes[idx].hadDealRequest = false;
	map->processes[idx].isValid = false;
	map->processNum--;
}

static void Init_Run_Enviroment(PCBC_SliceTime* pcbc, Process_Resource_Map *map) {
	PCB_SliceTime *temp = pcbc->ready_head;
	map->processNum = 0;
	int c = 0;
	while (temp != NULL) {
		//��ʼ���������Need:��ǰÿһ������������Ҫ����Դ��//
		Replace_Process_To_ResourceMap(map, temp, map->processNum);
		map->processNum++;
		if (map->processNum >= PROCESS_NUM)
			break;
		temp = temp->next;//ͣ���ڵ�������̵���
	}

	if (map->processNum == PROCESS_NUM && temp->next != pcbc->ready_tail) {
		printf("���̹���");
		assert(false);
	}

	//��ʼ�����������Դ�����ڽ��̱�����Ҫ����Դ
	map->resourceMax[0] = map->resourceavailable[0]=30;
	map->resourceMax[1] = map->resourceavailable[1] = 45;
	map->resourceMax[2] = map->resourceavailable[2] = 40;


	//�������Allocation����������ϵͳ��ÿһ����Դ�Ѿ������ÿһ�����̵���Դ��
	

}
void Free_Process(PCBC_SliceTime *pcbc, Process_Resource_Map *map,PCB_SliceTime *pcb) {

	//ʱ��Ƭ����֮ǰ��ɣ�����ɵĽ����ƶ�����������
	printf("���̣�%s�������С�\n", pcbc->run->name);
	pcbc->run->state = FINISH;
	pcbc->run->next = pcbc->finish;
	pcbc->finish = pcbc->run;
	ProcessWithResource *pcbr = Get_PCBR_ByProcess(map, pcb);
	pcbr->process = NULL;
	pcbr->isValid = false;
	
}


void Run_Process_SliceTime_N(PCBC_SliceTime* pcbc) {
	Process_Resource_Map process_map;
	ProcessWithResource *pcbr;
	Init_Run_Enviroment(pcbc, &process_map);
	Printf_Max(&process_map);

	while (pcbc->ready_head != NULL || pcbc->block != NULL) {
		//��ʼ�µ�һ��ʱ��Ƭ
		printf("start new slice time\n");
		int slice_time = 0;

		//�����������еĽ����ƶ�������������
		while (pcbc->block != NULL) {
			PCB_SliceTime *temp = pcbc->block;
			pcbc->block = pcbc->block->next;
			Add_Process_To_Ready(pcbc, temp);
		}
		Printf_PCBC_N(pcbc);
		//��ѡ��һ�����õģ����ᵼ�������Ľ���
		while (pcbc->ready_head != NULL) {
			PCB_SliceTime *pcb = pcbc->ready_head;
			ProcessWithResource *pcbr = Get_PCBR_ByProcess(&process_map, pcb);
			Generate_Request(pcbr); //����һ�����������Դ����
									//���ͨ���˰�ȫ���ԣ���ô���߳̿������У�����Ѱ��ѭ��
			if (Try_AllcateRequestResource_SystemIsSafe(&process_map, pcbr))
			{
				break;
			}
			else { //�����ý���
				printf("�������̡�%s��\n", pcbc->ready_head->name);
				pcbc->ready_head = pcbc->ready_head->next;
				if (pcbc->ready_head != NULL)
					printf("head name=%s\n", pcbc->ready_head->name);
				Add_Process_To_Block(pcbc, pcb);
			}
		}
		//ȫ������������
		if (pcbc->ready_head == NULL && pcbc->block != NULL)
		{
			printf("Error:ϵͳ����,��������");
			Printf_PCBC_N(pcbc);

			return;
		}
		//��ʼһ���µĽ��̣��ҵ�һ���������е��̣߳�����
		
		if (pcbc->ready_head == NULL) {
			printf("���н���\n");
			Printf_PCBC_N(pcbc);
			return;
		}
		pcbc->run = pcbc->ready_head;
		pcbc->run->state = RUN;
		Printf_PCBC_N(pcbc);

		while (slice_time < SLICE_TIME) {
			pcbc->run->take_cpu_time++;
			slice_time++;
			//���̽���

		}
		//ʱ��Ƭ����֮����ε���
		//���������Ҫ����Դ�Ѿ����㣬�����ý���
		if (IsRunFinish(&process_map, pcbc->run))
		{
			printf("���̡�%d���Ѿ���ɣ�\n",pcbc->run->name);
			pcbc->ready_head = pcbc->ready_head->next;
			Free_Process(pcbc, &process_map,pcbc->run);
		}
		else
		{
			//ʱ��Ƭ����֮�󣬽���δ��ɣ��ƶ�����������β��		
			pcbc->ready_head = pcbc->ready_head->next;
			Add_Process_To_Ready(pcbc, pcbc->run);
		}
	}
	printf("���о����������\n");
}

#endif