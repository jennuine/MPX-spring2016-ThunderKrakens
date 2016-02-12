/**
 * @file pcb.c
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R2
 *
 */

#include "pcb.h"
#include <string.h>
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

error_t suspend_pcb(struct pcb_struct * pcb_ptr){
  if (pcb_ptr == NULL)
    return E_NULL_PTR;

  pcb_ptr->is_suspended = true;
  return E_NOERROR;
}

error_t resume_pcb(struct pcb_struct * pcb_ptr){
  if (pcb_ptr == NULL)
    return E_NULL_PTR;

  pcb_ptr->is_suspended = false;
  return E_NOERROR;
}

struct pcb_struct * allocate_pcb()
{
  struct pcb_struct * a_pcb = sys_alloc_mem(sizeof(struct pcb_struct));

  if(a_pcb)
  {
    a_pcb->stack_base = sys_alloc_mem(SIZE_OF_STACK);
    a_pcb->stack_top = a_pcb->stack_top + SIZE_OF_STACK;
    a_pcb->prev = NULL;
    a_pcb->next = NULL;
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
    return E_NULL_PTR;

  if (remove_pcb(pcb_ptr) != E_NOERROR)
    return E_REMOVE_PCB;
  
  sys_free_mem(pcb_ptr->stack_base);

  sys_free_mem(pcb_ptr);

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
  struct pcb_struct * curr = ready_queue.head;

  while ( curr != NULL )
  {
    if (strcmp(curr->name, pName))
      return curr;

    curr = curr->next;
  }

  curr = blocked_queue.head;

  while ( curr != NULL )
  {
    if (strcmp(curr->name, pName))
      return curr;

    curr = curr->next;
  }

  return NULL;
}

/**
 * @name insert_pcb
 * @brief Inserts PCB into the appropriate queue.
 *
 * @param pcb_ptr 	The PCB pointer.
 * @return E_INSERT_PCB	The PCB was inserted.
 *
 */


error_t insert_pcb (struct pcb_struct * pcb_ptr)
{
    struct pcb_struct * current;

    //double check how to check enum equals -> It is the other way around
    if (pcb_ptr->running_state == ready)
    {
        current = ready_queue.head;

        while (current != ready_queue.tail)
        {
                if (current->priority <= pcb_ptr->priority && current->next->priority >pcb_priority
                 )
                {
	                 current->next = pcb_ptr->next;
	                 current->prev = pcb_ptr;
	
	                 pcb_ptr->next = current;
	                 pcb_ptr->next->prev = current;

		               return E_NOERROR;
                }

            current = current->next;
        }
    }

    //will need to do else if process_state.block and insert at tail
    
    else if (pcb_ptr->running_state == blocked)
    {
      
      
      
    }

	return E_INSERT_PCB_FAIL;
}

/**
 * @name remove_pcb
 * @brief Removes PCB from the queue it is currently in.
 *
 * @param pcb_ptr 	The PCB pointer.
 * @return E_REMOVE_PCB 	PCB removed
 * @return E_REMOVE_PCB_FAIL	Failed to remove PCB
 *
 */

error_t remove_pcb(struct pcb_struct * pcb_ptr)
{
	struct pcb_struct * current = find_pcb(pcb_ptr->name);

	if (pcb_ptr == NULL )
	{
  	return E_NOT_FOUND;
	}

	current->other_pcb->next->prev = current->other_pcb->prev;
	current->other_pcb->prev->next = current->other_pcb->next;
	free_pcb(current);

 	return E_NOERROR;

}
