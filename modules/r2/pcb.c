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

/**
 * R3 data
 */
#include "../r3/context.h"

/**
 * PCBs stored in priority order with highest priority at head 
 */
static struct pcb_queue ready_queue;

/**
 * PCBs stored in FIFO order
 */
static struct pcb_queue blocked_queue;

static char *enum_process_state[] = {"running", "ready", "blocked"};
static char *enum_process_suspended[] = {"suspended", "unsuspended"};
static char *enum_process_class[] = {"application", "system"};


/**
* PCB process states/statuses
*/
enum process_state
{
  running, /**< PCB in the running state. */
  ready, /**< PCB in the ready state. */
  blocked /**< PCB in the blocked state. */
} __attribute__ ((packed));

/**
* PCB process suspended or not suspended status.
*/
enum process_suspended
{
  true, /**< PCB process is suspended. */
  false /**< PCB process is not suspended. */
} __attribute__ ((packed));

/**
* Struct that will describe PCB Processes.
*/
struct pcb_struct
{
  char name[SIZE_OF_PCB_NAME]; /**< PCB's name. */
  enum process_class class; /**< PCB's class is an application or system process. */
  unsigned char priority; /**< PCB's priority an integer between 0 and 9. Processes with higher priority values execute before lower priority processes. */
  enum process_state running_state; /**< PCB's states are ready, running, or blocked. */
  enum process_suspended is_suspended; /**< PCB process is either suspended or not suspended. */
  unsigned char * stack_top; /**< Pointer to top of the stack. */
  unsigned char * stack_base; /**< Pointer to base of the stack. */
  struct pcb_struct * prev; /**< Pointer to the previous PCB in the queue */
  struct pcb_struct * next; /**< Pointer to the next PCB in the queue */
};

/**
* Queue structure that will store PCBs.
*/
struct pcb_queue
{
  int count; /**< The length of the queue. */
  struct pcb_struct * head; /**< Pointer to the start/head of the queue. */
  struct pcb_struct * tail; /**< Pointer to the end/tail of the queue. */
};

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

/**
 * @name  suspend_pcb
 * @brief Suspends the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 */
error_t suspend_pcb(struct pcb_struct * pcb_ptr){
  if (pcb_ptr == NULL)
    return E_NULL_PTR;

  pcb_ptr->is_suspended = true;
  return E_NOERROR;
}

/**
 * @name  resume_pcb
 * @brief Resumes the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 */
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
    a_pcb->stack_top = a_pcb->stack_base + SIZE_OF_STACK - sizeof(struct context);
    a_pcb->prev = NULL;
    a_pcb->next = NULL;
  }

  return a_pcb;
}

/**
 * @name  setup_pcb
 * @brief allocate a space for the PCB structure, setup the properties of the PCB.
 *    NOTE: pName must less than SIZE_OF_PCB_NAME character, pClass should be either "application" or "system"
 *    , and pPriority must within the range of [0, 9].
 *
 * @param   pName       Process Name (length < SIZE_OF_PCB_NAME).
 * @param   pClass      Process class (system or application).
 * @param   pPriority   Process priority (0 ~ 9).
 *
 * @return  NULL if error occured, otherwise, the pointer that point to the PCB structure.
 */
struct pcb_struct * setup_pcb(const char * pName, const enum process_class pClass, const unsigned char pPriority)
{
  if(!(strlen(pName) < SIZE_OF_PCB_NAME && pClass <= pcb_class_sys && pPriority <= 9 && !find_pcb(pName)))
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
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_INVPARA   The PCB probably had not been removed from queue before free it.
 *      E_FREEMEM   The memory space cannot be actually free, since the student_free had not been implemented yet.
 */
error_t free_pcb(struct pcb_struct * pcb_ptr)
{
  error_t error_code = E_NOERROR;

  if (pcb_ptr == NULL )
    return E_NOERROR; //Already free, return no error.
  else if(pcb_ptr->prev || pcb_ptr->next || ready_queue.head == pcb_ptr || blocked_queue.head == pcb_ptr || ready_queue.tail == pcb_ptr || blocked_queue.tail == pcb_ptr)
    return E_INVPARA; //The PCB probably had not been removed from queue before free it.

  error_code = sys_free_mem(pcb_ptr->stack_base) == -1 ? E_FREEMEM : E_NOERROR ;

  error_code = sys_free_mem(pcb_ptr) == -1 ? E_FREEMEM : E_NOERROR ;

  return error_code;
}

/**
 * @name find_pcb
 * @brief Will search all queues for a process named pName
 *
 * @param   pName  The char pointer to the desired searched name
 *
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
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has running status or abnormal data members.
 */
error_t insert_pcb (struct pcb_struct * pcb_ptr)
{
  struct pcb_struct * current;
  if(!pcb_ptr)
  {
    return E_NULL_PTR;
  }
  if ((pcb_ptr->running_state == running) || pcb_ptr->prev || pcb_ptr->next)
  {
    return E_INVPARA;
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

  return E_INVPARA;
}

/**
 * @name remove_pcb
 * @brief Removes PCB from the queue it is currently in.
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members.
 */
error_t remove_pcb(struct pcb_struct * pcb_ptr)
{
	if(!pcb_ptr)
  {
    return E_NULL_PTR;
  }
  else if(!pcb_ptr->prev && !pcb_ptr->next && (ready_queue.head != pcb_ptr && blocked_queue.head != pcb_ptr))
  { //This PCB is already not belonging to any queue, return no error.
    return E_NOERROR;
  }
  else if(!pcb_ptr->prev && !pcb_ptr->next && (ready_queue.head == pcb_ptr || blocked_queue.head == pcb_ptr))
  { //This PCB is the only one in the queue.
    if(pcb_ptr->running_state == blocked)
    {
      blocked_queue.head = blocked_queue.tail = NULL;
      blocked_queue.count--;
    }
    else
    {
      ready_queue.head = ready_queue.tail = NULL;
      ready_queue.count--;
    }
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else if(pcb_ptr->prev && pcb_ptr->next)
  { //Removes from middle
    pcb_ptr->prev->next = pcb_ptr->next;
    pcb_ptr->next->prev = pcb_ptr->prev;
    pcb_ptr->prev = pcb_ptr->next = NULL;
    if(pcb_ptr->running_state == blocked)
    {
      blocked_queue.count--;
    }
    else
    {
      ready_queue.count--;
    }
  }
  else if(pcb_ptr->prev && !pcb_ptr->next && (ready_queue.tail == pcb_ptr || blocked_queue.tail == pcb_ptr))
  { //Removes from tail
    pcb_ptr->prev->next = NULL;
    if(pcb_ptr->running_state == blocked)
    {
      blocked_queue.tail = pcb_ptr->prev;
      blocked_queue.count--;
    }
    else
    {
      ready_queue.tail = pcb_ptr->prev;
      ready_queue.count--;
    }
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else if(!pcb_ptr->prev && pcb_ptr->next && (ready_queue.head == pcb_ptr || blocked_queue.head == pcb_ptr))
  { //Removes from head
    pcb_ptr->next->prev = NULL;
    if(pcb_ptr->running_state == blocked)
    {
      blocked_queue.head = pcb_ptr->next;
      blocked_queue.count--;
    }
    else
    {
      ready_queue.head = pcb_ptr->next;
      ready_queue.count--;
    }
    pcb_ptr->prev = pcb_ptr->next = NULL;
  }
  else
  {
    return E_INVPARA;
  }

 	return E_NOERROR;

}

/**
 * @name show_pcb
 * @brief Displays the name, class, state, suspend status, and priority of a PCB.
 *
 * @param   pName 	The PCB pointer.
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 */
error_t show_pcb(struct pcb_struct * pcb_ptr)
{
  if (!pcb_ptr)
  {
    return E_NULL_PTR;
  }

  printf("\tName:\t%s\n", pcb_ptr->name);
  printf("\tClass:\t%s\n", enum_process_class[pcb_ptr->class]);
  printf("\tState:\t%s\n", enum_process_state[pcb_ptr->running_state]);
  printf("\tSuspended:\t%s\n", enum_process_suspended[pcb_ptr->is_suspended]);
  printf("\tPriority:\t%d\n\n",  pcb_ptr->priority);

  return E_NOERROR;
}

/**
 * @name show_blocked_processes
 * @brief displays all blocked processes and their attributes
 *
 * @return  VOID.
 */
void show_blocked_processes()
{
  struct pcb_struct * current = blocked_queue.head;

  while(current)
  {
    show_pcb(current);
    current = current->next;
  }
}

/**
 * @name show_ready_processes
 * @brief Displays all of the ready processes and their attributes.
 *
 * @return  VOID.
 */
void show_ready_processes()
{
  struct pcb_struct * current = ready_queue.head;

  while(current)
  {
    show_pcb(current);
    current = current->next;
  }
}

/**
 * @name show_all_processes
 * @brief Displays all of the processes and their attributes.
 *
 * @return   VOID.
 */
void show_all_processes()
{

  show_ready_processes();
  show_blocked_processes();

}



/**
 * @name block_pcb
 * @brief puts the given pcb into the blocked state and places it into the correct queue
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members (By "remove_pcb" or "insert_pcb").
 */
error_t block_pcb(struct pcb_struct * pcb_ptr)
{
  if (pcb_ptr == NULL)
  {
    return E_NULL_PTR;
  }
  error_t err = E_NOERROR;

  if((err = remove_pcb(pcb_ptr)) != E_NOERROR)
  {
    return err;
  }

  pcb_ptr->running_state = blocked;

  if((err = insert_pcb(pcb_ptr)) != E_NOERROR)
  {
    return err;
  }

  return err;
}



/**
 * @name unblock_pcb
 * @brief puts the given pcb into the unblocked state and places it into the correct queue
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members (By "remove_pcb" or "insert_pcb").
 */
error_t unblock_pcb(struct pcb_struct * pcb_ptr)
{
  if (pcb_ptr == NULL)
  {
    return E_NULL_PTR;
  }
  error_t err = E_NOERROR;

  if((err = remove_pcb(pcb_ptr)) != E_NOERROR)
  {
    return err;
  }

  pcb_ptr->running_state = ready;

  if((err = insert_pcb(pcb_ptr)) != E_NOERROR)
  {
    return err;
  }

  return err;
}

/**
 * @name set_pcb_priority
 * @brief Sets the priority of the selected PCB
 *
 * @param   pcb_ptr 	The PCB pointer.
 * @param   pPriorty  The assigned priorirty
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The pPriority is out of range.
 *                  Or, the given PCB has abnormal data members (By "remove_pcb" or "insert_pcb").
 */
error_t set_pcb_priority(struct pcb_struct * pcb_ptr, const unsigned char pPriority)
{
  if (pcb_ptr == NULL)
  {
		return E_NULL_PTR;
	}
  error_t errno = E_NOERROR;
  if(pcb_ptr->priority == pPriority)
  {
    //Do nothing here.
  }
  else if (pPriority <= 9)
  {
    errno = remove_pcb(pcb_ptr);
    if(!errno)
    {
      pcb_ptr->priority = pPriority;
      errno = insert_pcb(pcb_ptr);
    }
  }
  else
  {
    errno = E_INVPARA;
  }

  return errno;
}

/**
 * @name get_running_process
 * @brief gets a unsuspended and unblocked process from the front of the queue, and sets it to running state.
 *
 * @param   None
 *
 * @return NULL if there is no process available, otherwise, the pointer that point to the PCB structure.
 */
struct pcb_struct * get_running_process()
{
  struct pcb_struct * result = NULL;
  struct pcb_struct * currentPCB = ready_queue.head;
  unsigned char bIsFound = 0;
  while(currentPCB && !bIsFound)
  {
    if(currentPCB->is_suspended == false)
    {
      bIsFound = 1;
      if((remove_pcb(currentPCB)) == E_NOERROR)
      {
        //printf("*Debug Log* Return process: %s\n", currentPCB->name);
        currentPCB->running_state = running;
        result = currentPCB;
      }
    }
    else
    {
      currentPCB = currentPCB->next;
    }
  }
  return result;
}

/**
 * @name save_running_process
 * @brief sets the running process to ready state, and inserts it to the ready queue.
 *
 * @param   pcb_ptr The pointer to the PCB.
 * @param   new_stack_top The pointer to the new stack top.
 *
 * @return The error code.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members (By "insert_pcb").
 */
error_t save_running_process(struct pcb_struct * pcb_ptr, struct context * new_stack_top)
{
  if(!pcb_ptr)
    return E_NULL_PTR;
    
  //printf("*Debug Log* save process: %s\n", pcb_ptr->name);
  pcb_ptr->running_state = ready;
  pcb_ptr->stack_top = (unsigned char *)new_stack_top;
  
  return insert_pcb(pcb_ptr);
}

/**
 * @name get_stack_top
 * @brief gets the pointer to the stack top of the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB.
 *
 * @return NULL if the pcb_ptr is NULL, otherwise, the pointer that point to the stack top of the specific PCB.
 */
unsigned char * get_stack_top(struct pcb_struct * pcb_ptr)
{
  if(!pcb_ptr)
    return NULL;
    
  return pcb_ptr->stack_top;
}

/**
 * @name get_stack_base
 * @brief gets the pointer to the stack base of the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB.
 *
 * @return NULL if the pcb_ptr is NULL, otherwise, the pointer that point to the stack base of the specific PCB.
 */
unsigned char * get_stack_base(struct pcb_struct * pcb_ptr)
{
  if(!pcb_ptr)
    return NULL;
    
  return pcb_ptr->stack_base;
}

/**
 * @name shutdown_pcb
 * @brief called when system is going to shutdown, removes all PCBs, free all PCBs.
 * 
 * @return VOID
 */
void shutdown_pcb()
{
  struct pcb_struct * curr = ready_queue.head;
  struct pcb_struct * temp = NULL;
  while ( curr != NULL )
  {
    temp = curr;
    curr = curr->next;
    remove_pcb(temp);
    free_pcb(temp);
  }

  curr = blocked_queue.head;

  while ( curr != NULL )
  {
    temp = curr;
    curr = curr->next;
    remove_pcb(temp);
    free_pcb(temp);
  }
}