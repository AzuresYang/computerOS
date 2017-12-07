#pragma once
#ifndef _MEMALLOC_H_
#define _MEMALLOC_H_
#include <malloc.h>
#include<stdio.h>
#define HEAPSIZE 640
#define MAXINT 2147483647  //just 
typedef enum BLOCK_STATE{
	FREE,USE
}BLOCK_STATE;

typedef enum FIND_FIT_BLOCK_STRATEGY {
	FIRST_SUIT,BEST_SUIT
}FIND_FIT_BLOCK_STRATEGY;

typedef struct  Block {
	int idx;
	int size;
	BLOCK_STATE state;
	struct Block* next;
	struct Block* pre;
}Block;


static char *memc_heap;
static Block *mem_heap;
static FIND_FIT_BLOCK_STRATEGY find_fit_block_srategy = FIRST_SUIT;

void printf_heap_info() {
	Block *block = mem_heap->next;
	int num = 0;
	int heap_free_size = 0;
	printf("heap info：\n");
	while (block != NULL)
	{
		num++;
		heap_free_size += block->size;
		printf("block【%d】，start[%d], size[%d]\n", num, block->idx, block->size);
		block = block->next;
	}
	printf("堆剩余大小：%d\n", heap_free_size);


}
void mem_init(void) {
	memc_heap =(char*)malloc(HEAPSIZE);
	mem_heap = (Block*)malloc(sizeof(Block));
	mem_heap->idx = -1;
	mem_heap->size = 0;
	mem_heap->state = USE;
	mem_heap->pre = NULL;
	mem_heap->next = (Block*)malloc(sizeof(Block));

	mem_heap->next->pre = mem_heap;
	mem_heap->next->next = NULL;
	mem_heap->next->size = HEAPSIZE;
	mem_heap->next->idx = 0;
	mem_heap->next->state = FREE;
}

void place(Block *block,int size) {
	if (block == NULL)
		return;
	block->state = USE;
	if (block->size > size)
	{
		Block *newblock = (Block*)malloc(sizeof(Block));
		newblock->pre = block->pre;
		newblock->pre->next = newblock;

		newblock->next = block->next;
		if(block->next != NULL)
			block->next->pre = newblock;
		
		newblock->idx = block->idx + size;
		newblock->size = block->size - size;
		newblock->state = FREE;

		block->size = size;
		block->pre = NULL;
		block->next = NULL;
	}
	else {
		block->pre->next = block->next;
		if (block->next != NULL)
			block->next->pre = block->pre;
	}
}

//合并空闲的空间
void merge_heap_block() {
	Block *block = mem_heap->next;
	Block *free_temp;
	while (block != NULL)
	{
		if (block->next == NULL)
			return;
		if (block->idx + block->size == block->next->idx)
		{
			free_temp = block->next;
			block->size += block->next->size;
			block->next = block->next->next;
			free(free_temp);
		}
		else
			block = block->next;
	}
}

Block* find_fit_first_fit(int size) {
	if (mem_heap->next == NULL)
		return NULL;
	Block *fit = mem_heap->next;
	while (fit != NULL) {
		if (fit->size >= size)
			return fit;
		else
			fit = fit->next;
	}
	return NULL;
}

Block* find_fit_best_fit(int size)
{
	if (mem_heap->next == NULL)
		return NULL;
	Block *best = NULL;
	int sizeDiff = MAXINT;
	Block *temp = mem_heap->next;
	while (temp != NULL)
	{
		if (temp->size == size)
			return temp;
		
		int temp_diff = temp->size - size;
		if (temp_diff > 0 && temp_diff < sizeDiff)
		{
			best = temp;
			sizeDiff = temp_diff;
		}
		temp = temp->next;
	}
	return best;
	
}

Block* find_fit(int size, FIND_FIT_BLOCK_STRATEGY stra)
{
	switch (stra)
	{
		case BEST_SUIT:  return find_fit_best_fit(size);
		case FIRST_SUIT: return find_fit_first_fit(size);
		default:printf("内存分配寻找策略有误！没有改选项"); break;
	}
	return NULL;
}
Block* find_fit(int size) {
	return find_fit(size, find_fit_block_srategy);
}

void change_find_fit_strategy(FIND_FIT_BLOCK_STRATEGY stra)
{
	find_fit_block_srategy = stra;
}

Block *mem_malloc(int size) {
	if (size <= 0) {
		printf("不能申请小于0的空间");
		return NULL;
	}
	if (size > HEAPSIZE)
	{
		printf("不能申请大于%d的空间，因为大于内存大小",HEAPSIZE);
		return NULL;
	}
	Block *fit_block = find_fit(size);
	if (fit_block != NULL)
	{
		place(fit_block, size);
		return fit_block;
	}

	//重新合并内存空间
	merge_heap_block();
	fit_block = find_fit(size);
	if (fit_block != NULL)
	{
		place(fit_block, size);
		return fit_block;
	}else
	{
		printf("申请空间大于可用内存\n");
		return NULL;
	}

}
 
int  mem_heap_size() {
	Block *block = mem_heap->next;
	int heap_free_size = 0;
	while (block != NULL)
	{
		heap_free_size += block->size;
		block = block->next;
	}
	return heap_free_size;
}
void mem_free(Block *block) {
	if (block == NULL)
		return;
	Block *pos = mem_heap->next;
	block->next = NULL;
	block->pre = NULL;
	/*
		这里应该清除block中的数据
	*/
	block->state = FREE;
	//
	if (pos == NULL)
	{
		mem_heap->next = mem_heap->next;
	}
	else
	{
		//如果block处于堆头
		if (block->idx < mem_heap->next->idx)
		{
			pos = mem_heap;
		}
		else {
			while (block->idx > pos->idx)
			{
				if (pos->next == NULL)
					break;
				else
					pos = pos->next;
			}
		}
		
		//block处于末尾
		if (pos->next == NULL)
		{
			pos->next = block;
			block->pre = pos;
		}//block处于中间			
		else
		{
			block->next = pos->next;
			pos->next->pre = block;

			block->pre = pos;
			pos->next = block;
		}
	}
}

void clear() {
	Block *pre = mem_heap;
	Block *temp = pre->next;
	while (temp != NULL) {
		free(pre);
		pre = temp;
		temp = temp->next;
	}
	free(pre);
	free(memc_heap);
}



#endif