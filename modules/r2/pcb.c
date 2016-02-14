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

/**
 * @name  pcb_init
 * @brief Initiates the PCB queues
 *
 */
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

/**
 * @name  allocate_pcb
 * @brief allocate a space for the PCB structure.
 *
 * @return  The pointer that point to the PCB structure.
 */
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

/**
 * @name  setup_pcb
 * @brief allocate a space for the PCB structure, setup the properties of the PCB.
 *    NOTE: pName must less than 10 character, pClass should be either "application" or "system"
 *    , and pPriority must within the range of [0, 9].
 *
 * @return  NULL if error occured, otherwise, the pointer that point to the PCB structure.
 */
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
  error_t error_code = E_NOERROR;

  if (pcb_ptr == NULL )
    return E_NOERROR;

  if ((error_code = remove_pcb(pcb_ptr)) != E_NOERROR)
    return error_code;

  sys_free_mem(pcb_ptr->stack_base);

  sys_free_mem(pcb_ptr);

  return error_code;
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
    if (!strcmp(curr->name, pName))
      return curr;

    curr = curr->next;
  }

  curr = blocked_queue.head;

  while ( curr != NULL )
  {
    if (!strcmp(curr->name, pName))
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
  if (pcb_ptr->running_state == running || pcb_ptr->prev || pcb_ptr->next)
  {
    return E_INSERT_PCB_FAIL;
  }
  //double check how to check enum equals -> It is the other way around
  if (pcb_ptr->running_state == ready)
  {
    if(!ready_queue.count && !ready_queue.head && !ready_queue.tail)
    { //Inserts the first one.
      ready_queue.head = ready_queue.tail = pcb_ptr;
      ready_queue.count++;
      return E_NOERROR;
    }

    current = ready_queue.head;
    while (current)
    { //Processes with lower priority values should execute after higher priority processes
      if (current->priority < pcb_ptr->priority)
      {
        pcb_ptr->next = current;
        pcb_ptr->prev = current->prev;

        if (current->prev)		//inserts at middle
          current->prev->next = pcb_ptr;
        else					//inserts at head.
          ready_queue.head = pcb_ptr;

        current->prev = pcb_ptr;
        ready_queue.count++;
        return E_NOERROR;
      }
      else if (!current->next && ready_queue.tail == current)	//inserts at tail.
      {
        pcb_ptr->next = NULL;
        pcb_ptr->prev = current;
        current->next = pcb_ptr;
        ready_queue.tail = pcb_ptr;
        ready_queue.count++;
        return E_NOERROR;
      }

      current = current->next;
    }
  }
  else if (pcb_ptr->running_state == blocked) //will need to do else if process_state.block and insert at tail
  {
    if(!blocked_queue.count && !blocked_queue.head && !blocked_queue.tail)
    { //Inserts the first one.
      blocked_queue.head = blocked_queue.tail = pcb_ptr;
      blocked_queue.count++;
      return E_NOERROR;
    }
    current = blocked_queue.tail;
    pcb_ptr->next = NULL;
    pcb_ptr->prev = current;
    current->next = pcb_ptr;
    blocked_queue.tail = pcb_ptr;
    blocked_queue.count++;
    return E_NOERROR;
  }

return E_PROGERR;
}

/**
 * @name remove_pcb
 * @brief Removes PCB from the queue it is currently in.
 *
 * @param pcb_ptr 	The PCB pointer.
 * @return The error code.
 */

error_t remove_pcb(struct pcb_struct * pcb_ptr)
{
	if(!pcb_ptr)
  {
    return E_NULL_PTR;
  }

  if(pcb_ptr->prev && pcb_ptr->next)
  { //Removes from middle
    pcb_ptr->prev->next = pcb_ptr->next;
    pcb_ptr->next->prev = pcb_ptr->prev;
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else if(pcb_ptr->prev && !pcb_ptr->next && (ready_queue.tail == pcb_ptr || blocked_queue.tail == pcb_ptr))
  { //Removes from tail
    pcb_ptr->prev->next = NULL;
    if(pcb_ptr->running_state == blocked)
      blocked_queue.tail = pcb_ptr->prev;
    else
      ready_queue.tail = pcb_ptr->prev;
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else if(!pcb_ptr->prev && pcb_ptr->next && (ready_queue.head == pcb_ptr || blocked_queue.head == pcb_ptr))
  { //Removes from head
    pcb_ptr->next->prev = NULL;
    if(pcb_ptr->running_state == blocked)
      blocked_queue.head = pcb_ptr->next;
    else
      ready_queue.head = pcb_ptr->next;
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else
  {
    return E_INVPARA;
  }

 	return E_NOERROR;

}
