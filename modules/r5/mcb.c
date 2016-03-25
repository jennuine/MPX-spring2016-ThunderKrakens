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

struct mcb_list {
    struct mcb *prev;
    struct mcb *next;
};

struct mcb_list free_mem_list;
struct mcb_list allocated_mem_list;


void init_heap(u32int size) 
{
    start_of_memory = kmalloc(size + sizeof(struct cmcb) + sizeof(struct lmcb));
    
    struct mcb * block = sys_alloc_mem(sizeof(struct mcb));
    struct cmcb * complete = sys_alloc_mem(sizeof(struct cmcb));
    struct lmcb * limited = sys_alloc_mem(sizeof(struct lmcb));

    complete->type = free;
    limited->type = free;
    block->complete_mcb = complete;
    block->limited_mcb = limited;

    // slides: 'put a cmcb at top of heap & lmcb at bottom both type free'
    //call make_heap ?
    
    allocated_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    allocated_mem_list.next = NULL;
    allocated_mem_list.prev = NULL;
    
    free_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    free_mem_list.next = block;
    free_mem_list.prev = NULL;
}