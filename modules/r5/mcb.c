/**
 * @file mcb.c
 * @author Thunder Krakens
 * @date April 8th, 2016
 * @brief Memory Control Block
 * @version R5
 *
 */

#include "mcb.h"
#include <system.h>
#include <string.h>
#include <mem/heap.h>
#include "../mpx_supt.h"


u32int start_of_memory;

//struct mcb_list {
//    struct mcb *prev;
//    struct mcb *next;
//};

struct mcb * free_mem_list;
struct mcb * allocated_mem_list;

static void init_mem_block(struct mcb * start_pos, u32int size, enum mcb_type type)
{
    start_pos->complete_mcb = (struct cmcb *)start_pos;
    start_pos->limited_mcb = (struct lmcb *)((u32int)start_pos + sizeof(struct cmcb) + size);
    start_pos->complete_mcb->begin_address = (void *)((u32int)start_pos + sizeof(struct cmcb));
    
    start_pos->complete_mcb->size = start_pos->limited_mcb->size = size;
    start_pos->complete_mcb->type = start_pos->limited_mcb->type = type;
    
    start_pos->complete_mcb->prev = start_pos->complete_mcb->next = NULL;
}

void init_heap(u32int size) 
{
    start_of_memory = kmalloc(sizeof(struct cmcb) + size + sizeof(struct lmcb));
    struct mcb * first_free_mem = (struct mcb *)start_of_memory;
    
    init_mem_block(first_free_mem, size, free);
    
    free_mem_list = first_free_mem;
    allocated_mem_list = NULL;
    //struct mcb * block = sys_alloc_mem(sizeof(struct mcb));
    //struct cmcb * complete = sys_alloc_mem(sizeof(struct cmcb));
    //struct lmcb * limited = sys_alloc_mem(sizeof(struct lmcb));

    //complete->type = free;
    //limited->type = free;
    //block->complete_mcb = complete;
    //block->limited_mcb = limited;

    // slides: 'put a cmcb at top of heap & lmcb at bottom both type free'
    //call make_heap ?
    
    //allocated_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    //allocated_mem_list.next = NULL;
    //allocated_mem_list.prev = NULL;
    
    //free_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    //free_mem_list.next = block;
    //free_mem_list.prev = NULL;
}