/**
 * @file pcb.c
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R2
 *
 */

#include "pcb.h"
#include "../mpx_supt.h"
#include "string.h"

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

/**
 * @name free_pcb
 * @brief Frees all memory associated with given PCB, including the PCB itself, the stack, etc, with sys_free_mem()
 *
 * @param pcb_ptr    The PCB pointer.
 * @return error_t code
 */
error_t free_pcb(struct pcb_struct * pcb_ptr)
{
  if (pcb_ptr == NULL )
    return E_NOT_FOUND;

  if (sys_free_mem(pcb_ptr->stack_top) == -1)
    return E_FREEMEM;
  if (sys_free_mem(pcb_ptr->stack_base) == -1)
    return E_FREEMEM;

  struct pcb_queue_node * curr = pcb_ptr->other_pcb->head;

  while ( curr != NULL )
  {
    curr = curr->next;

    if (sys_free_mem(curr->prev) == -1)
      return E_FREEMEM;
  }

  if (remove_pcb(pcb_ptr) != E_NOERROR)
    return E_REMOVE_PCB;

  if (sys_free_mem(pcb_ptr) == -1)
    return E_FREEMEM;

  return E_NOERROR;
}

/**
 * @name pcb_struct
 * @brief Will search all queues for a process named pName
 *
 * @param pName  The char pointer to the desired searched name
 * @return PCB pointer if found, NULL if PCB is not found
 */
struct pcb_struct * find_pcb(const char * pName)
{
  struct pcb_queue_node * curr = ready_queue.head;

  while ( curr != NULL )
  {
    if (strcmp((curr->actual_pcb).name, pName))
      return &curr->actual_pcb;

    curr = curr->next;
  }

  curr = blocked_queue.head;

  while ( curr != NULL )
  {
    if (strcmp((curr->actual_pcb).name, pName))
      return &curr->actual_pcb;

    curr = curr->next;
  }

  return NULL;
}
