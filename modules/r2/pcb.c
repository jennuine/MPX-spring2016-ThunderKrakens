/**
 * @file pcb.c
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R2
 *
 */

#include "pcb.h"
#include<string.h>
#include "../mpx_supt.h"

static struct pcb_queue ready_queue;
static struct pcb_queue blocked_queue;

void pcb_init()
{
  ready_queue.count = 0;
  ready_queue.head = NULL;
  ready_queue.tail = NULL;

  blocked_queue.count = 0;
  blocked_queue.head = NULL;
  blocked_queue.tail = NULL;
}

struct pcb_struct * allocate_pcb()
{
  struct pcb_struct * a_pcb = sys_alloc_mem(sizeof(struct pcb_struct));

  if(a_pcb)
  {
    a_pcb->stack_top = sys_alloc_mem(SIZE_OF_STACK);
    a_pcb->stack_base = a_pcb->stack_top + SIZE_OF_STACK;
    a_pcb->other_pcb = NULL;
  }

  return a_pcb;
}

struct pcb_struct * setup_pcb(const char * pName, const enum process_class pClass, const unsigned char pPriority)
{
  if(!(strlen(pName) < 10 && pClass <= system && pPriority <= 9))
    return NULL;

  struct pcb_struct * a_pcb = allocate_pcb();

  if(a_pcb)
  {
    strcpy(a_pcb->name, pName);
    a_pcb->class = pClass;
    a_pcb->priority = pPriority;
    a_pcb->running_state = ready;
    a_pcb->is_suspended = false;
  }

  return a_pcb;
}
