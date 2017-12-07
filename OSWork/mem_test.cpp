#include "memalloc.h"
#include<stdlib.h>

#define  BLOCKSIZE 100
Block* malloc_blocks[BLOCKSIZE];
void test1() {
	Block *block_130 = mem_malloc(130);
	Block *block_60 = mem_malloc(60);
	Block *block_100 = mem_malloc(100);
	mem_free(block_60);
	mem_free(block_130);
	printf_heap_info();
}

void test2() {
	Block *block_400 = mem_malloc(400);
	Block *block_60 = mem_malloc(60);
	Block *block_100 = mem_malloc(100);
	printf_heap_info();
	mem_free(block_400);
	printf("回收之后");
	printf_heap_info();
	block_400 = mem_malloc(400);
//	mem_free(block_130);
	printf_heap_info();

}

void printf_menu()
{

}

void init()
{
	mem_init();
	for (int i = 0; i < BLOCKSIZE; i++)
		malloc_blocks[i] = NULL;
}
void printf_malloc_blocks()
{
	for (int i = 0; i < BLOCKSIZE; i++)
	{
		if (malloc_blocks[i] == NULL)
			break;
		printf("Block[%d]:size[%d]		",i, malloc_blocks[i]->size);
		if (i % 5 == 0)
			printf("\n");
	}
	printf("\n");
}

int find_last() {
	for (int i = 0; i < BLOCKSIZE; i++)
	{
		if (malloc_blocks[i] == NULL)
			return i;
	}
	return 0;
}
void memmalloc_blocks() {
	int size = 0;
	printf("输入申请大小:");
	scanf_s("%d", &size);
	printf("\n");
	int idx = find_last();
	malloc_blocks[idx] = mem_malloc(size);
	if (malloc_blocks[idx]== NULL)
	{
		printf_heap_info();
		printf("内存分配失败，检查你的参数\n");
	}
	else {
		printf("分配成功！\n");
	}
}

void memfree_blocks() {
	int idx = 0;
	printf("输入free的块:");
	scanf_s("%d", &idx); 
	printf("\n");
	if (idx < 0 || idx >= BLOCKSIZE)
	{
		printf("输入的参数不当！");
		return;
	}
	if (malloc_blocks[idx] == NULL)
		return;
	mem_free(malloc_blocks[idx]);
	malloc_blocks[idx] = NULL;
	for (int i = idx + 1; i+1 < BLOCKSIZE && malloc_blocks[i] != NULL; i++)
	{
		malloc_blocks[i] = malloc_blocks[i + 1];
	}
}
void menu() {
	char input[10];
	while (input[0] != 'q')
	{
		printf("\n");
		printf("				------------内存调度算法-------------------\n");
		printf("							功能菜单\n");
		printf("				1.选择首次适应匹配内存\n");
		printf("				2.选择最佳适应匹配内存\n");
		printf("				3.申请内存\n");
		printf("				4.释放内存\n");
		printf("				5.查看内存信息\n");
		printf("				6.退出\n");
		printf("				内存大小：可用大小[%d]。目前已经申请的内存块:\n",mem_heap_size());
		printf_malloc_blocks();

		printf("输入功能：");
		scanf_s("%s", input, 10);
		switch (input[0])
		{
			case '1':  change_find_fit_strategy(FIRST_SUIT); printf("修改成功\n"); break;
			case '2':  change_find_fit_strategy(FIRST_SUIT); printf("修改成功\n"); break;
			case '3':  memmalloc_blocks(); break;
			case '4':  memfree_blocks(); break;
			case '5':  printf_heap_info(); break;
			case '6': break;
			default: printf("输入有误！"); break;
		}
	}
	printf("结束\n");
}

int main() {
	init();
	menu();
	printf("结束");
	clear();
	system("pause");
	return 0;
} 
