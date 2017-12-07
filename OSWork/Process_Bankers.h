#ifndef  _PROCESS_BANKERS_H_
#define  _PROCESS_BANKERS_H_
#include <malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>  
#include <assert.h>

#define PROCESS_NAME_LENGTH 20
#define SLICE_TIME 3

#define RESOURCE_MAX 30  //最大资源数
#define RESOURCE_SIZE 3
#define PROCESS_NUM 5

int g_resource_max[PROCESS_NUM][RESOURCE_SIZE]; //每个进程的最大资源需求
int g_resource_allocation[PROCESS_NUM][RESOURCE_SIZE]; //每个进程已经分配到的资源数量
int g_resource_available[RESOURCE_SIZE];//系统可用资源数量
int g_resource_need[PROCESS_NUM][RESOURCE_SIZE]; //每个进程所需资源量

typedef enum {
	READY, FINISH, RUN,BLOCK
}PROCESS_STATE;

typedef struct PCB_SliceTime {
	char name[PROCESS_NAME_LENGTH];
	int pid;
	PCB_SliceTime *next;
	int resources[RESOURCE_SIZE];
	int access_time; //到达时间
	int take_cpu_time; //占领CPU时间
	int process_time;//线程时间
	PROCESS_STATE state; //状态
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
	printf("进程资源管理模块中没有该进程:%s",process->name);
	assert(false);
	return NULL;
}
static ProcessWithResource* Get_PCBR_ByIdx(Process_Resource_Map *map, int idx) {
	if (idx < 0 || idx >= PROCESS_NUM)
	{
		printf("资源中，进程下标过大：%d", idx);
		assert(false);
		return NULL;
	}
	for (int i = 0; i < PROCESS_NUM; i++)
		if (map->processes[i].process != NULL && map->processes[i].idx == idx)
			return &map->processes[i];

	printf("进程资源管理模块中没有该下标进程:%d", idx);
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

		//分配资源
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
				 //资源不足
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
	
	//找到在运行的进程
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
			work[j] += pcbrs[idx]->allcation[j]; //取回资源
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
		printf("生成一个请求\n");
		for (int i = 0; i < RESOURCE_SIZE; i++)
		{
			int val = rand() % (pcbr->need[i] + 5);
			pcbr->request[i] = val > pcbr->need[i] ? pcbr->need[i] : val;
			printf("进程[%s]请求资源[%d]数量[%d]---目前所需资源数量[%d]\n", pcbr->process->name, i, pcbr->request[i], pcbr->need[i]);
		}
		pcbr->hadDealRequest = false;
	}
}
static bool Try_AllcateRequestResource_SystemIsSafe(Process_Resource_Map *map, ProcessWithResource* pcbr) {
	printf("进程[%s]所需资源数量[%d,%d,%d]\n", pcbr->process->name, pcbr->need[0], pcbr->need[1], pcbr->need[2]);
	printf("进程[%s]请求资源数量[%d,%d,%d]\n", pcbr->process->name, pcbr->request[0], pcbr->request[1], pcbr->request[2]);
	for (int i = 0; i < RESOURCE_SIZE; i++)
	{
		map->resourceavailable[i] -= pcbr->request[i];
		pcbr->allcation[i] += pcbr->request[i];
		if (pcbr->request[i] > pcbr->need[i])
		{
			printf("进程[%s]请求资源[%d]数量[%d]大于所需资源数量[%d]\n", pcbr->process->name, i, pcbr->request[i], pcbr->need[i]);
			return false;
		}
		else
			pcbr->need[i] -= pcbr->request[i];
	}
	//系统任然安全
	if (Is_SystemSafe(map))
	{
		pcbr->hadDealRequest = true;
		
		//该请求安全
		printf("请求通过\n");
		printf("进程[%s]目前所需资源数量[%d,%d,%d]\n", pcbr->process->name, pcbr->need[0], pcbr->need[1], pcbr->need[2]);
		return true;
	}
	else
	{
		//还原
		for (int i = 0; i < RESOURCE_SIZE; i++)
		{
			map->resourceavailable[i] += pcbr->request[i];
			pcbr->allcation[i] -= pcbr->request[i];
			pcbr->need[i] += pcbr->request[i];
		}
		printf("请求不通过\n");
		return false;
	}

}

//从Map中替代一个Process
static void Replace_Process_To_ResourceMap(Process_Resource_Map *map,PCB_SliceTime *pcb, int idx) {
	map->processes[idx].process = pcb;
	map->processes[idx].idx = map->processNum;
	map->processes[idx].hadDealRequest = true;
	map->processes[idx].isValid = true;

	//初始化资源
	for (int i = 0; i < RESOURCE_SIZE; i++) {
		map->processes[idx].allcation[i] = 0;
		map->processes[idx].need[i] = pcb->resources[i]; //资源初始化
	
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
		//初始化需求矩阵Need:当前每一个进程尚且需要的资源：//
		Replace_Process_To_ResourceMap(map, temp, map->processNum);
		map->processNum++;
		if (map->processNum >= PROCESS_NUM)
			break;
		temp = temp->next;//停留在第五个进程当中
	}

	if (map->processNum == PROCESS_NUM && temp->next != pcbc->ready_tail) {
		printf("进程过多");
		assert(false);
	}

	//初始化最大需求资源：等于进程本身需要的资源
	map->resourceMax[0] = map->resourceavailable[0]=30;
	map->resourceMax[1] = map->resourceavailable[1] = 45;
	map->resourceMax[2] = map->resourceavailable[2] = 40;


	//分配矩阵Allocation：它定义了系统总每一类资源已经分配给每一个进程的资源数
	

}
void Free_Process(PCBC_SliceTime *pcbc, Process_Resource_Map *map,PCB_SliceTime *pcb) {

	//时间片结束之前完成，则完成的进程移动至结束队列
	printf("进程：%s结束运行。\n", pcbc->run->name);
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
		//开始新的一个时间片
		printf("start new slice time\n");
		int slice_time = 0;

		//将阻塞队列中的进程移动到就绪队列中
		while (pcbc->block != NULL) {
			PCB_SliceTime *temp = pcbc->block;
			pcbc->block = pcbc->block->next;
			Add_Process_To_Ready(pcbc, temp);
		}
		Printf_PCBC_N(pcbc);
		//挑选出一个可用的，不会导致死锁的进程
		while (pcbc->ready_head != NULL) {
			PCB_SliceTime *pcb = pcbc->ready_head;
			ProcessWithResource *pcbr = Get_PCBR_ByProcess(&process_map, pcb);
			Generate_Request(pcbr); //产生一个随机请求资源队列
									//如果通过了安全测试，那么该线程可以运行，跳出寻找循环
			if (Try_AllcateRequestResource_SystemIsSafe(&process_map, pcbr))
			{
				break;
			}
			else { //阻塞该进程
				printf("阻塞进程【%s】\n", pcbc->ready_head->name);
				pcbc->ready_head = pcbc->ready_head->next;
				if (pcbc->ready_head != NULL)
					printf("head name=%s\n", pcbc->ready_head->name);
				Add_Process_To_Block(pcbc, pcb);
			}
		}
		//全部都被阻塞了
		if (pcbc->ready_head == NULL && pcbc->block != NULL)
		{
			printf("Error:系统死锁,跳出运行");
			Printf_PCBC_N(pcbc);

			return;
		}
		//开始一个新的进程，找到一个可以运行的线程，并且
		
		if (pcbc->ready_head == NULL) {
			printf("运行结束\n");
			Printf_PCBC_N(pcbc);
			return;
		}
		pcbc->run = pcbc->ready_head;
		pcbc->run->state = RUN;
		Printf_PCBC_N(pcbc);

		while (slice_time < SLICE_TIME) {
			pcbc->run->take_cpu_time++;
			slice_time++;
			//进程结束

		}
		//时间片结束之后，如何调度
		//如果所有需要的资源已经满足，弹出该进程
		if (IsRunFinish(&process_map, pcbc->run))
		{
			printf("进程【%d】已经完成；\n",pcbc->run->name);
			pcbc->ready_head = pcbc->ready_head->next;
			Free_Process(pcbc, &process_map,pcbc->run);
		}
		else
		{
			//时间片结束之后，进程未完成，移动至就绪队列尾部		
			pcbc->ready_head = pcbc->ready_head->next;
			Add_Process_To_Ready(pcbc, pcbc->run);
		}
	}
	printf("所有经常运行完毕\n");
}

#endif