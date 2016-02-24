/**
 * @file pcb.h
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R2
 *
 */

#ifndef _PCB_H
#define _PCB_H

#include "../errno.h"

/* @brief Represents the process class of application process. */
#define APP_PROCESS 10
/* @brief Represents the process class of system process. */
#define SYS_PROCESS 20
/* @brief The defualt size of the stack for the PCB */
#define SIZE_OF_STACK 1024

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
* PCB process class types.
*/
enum process_class
{
  application, /**< Process is an application process. */
  system /**< Process is a system process. */
} __attribute__ ((packed));

struct pcb_struct;
struct pcb_queue;

/**
* Struct that will describe PCB Processes.
*/
struct pcb_struct
{
  char name[10]; /**< PCB's name. */
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
void pcb_init();

/**
 * @name  allocate_pcb
 * @brief allocate a space for the PCB structure.
 *
 * @return  The pointer that point to the PCB structure.
 */
struct pcb_struct * allocate_pcb();

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
 */
error_t free_pcb(struct pcb_struct * pcb_ptr);

/**
 * @name  setup_pcb
 * @brief allocate a space for the PCB structure, setup the properties of the PCB.
 *    NOTE: pName must less than 10 character, pClass should be either "application" or "system"
 *    , and pPriority must within the range of [0, 9].
 * 
 * @param   pName       Process Name (length < 10).
 * @param   pClass      Process class (system or application).
 * @param   pPriority   Process priority (0 ~ 9).
 * 
 * @return  NULL if error occured, otherwise, the pointer that point to the PCB structure.
 */
struct pcb_struct * setup_pcb(const char * pName, const enum process_class pClass, const unsigned char pPriority);

/**
 * @name find_pcb
 * @brief Will search all queues for a process named pName
 *
 * @param   pName  The char pointer to the desired searched name
 * 
 * @return PCB pointer if found, NULL if PCB is not found
 */
struct pcb_struct * find_pcb(const char * pName);

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
error_t insert_pcb(struct pcb_struct * pcb_ptr);

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
error_t remove_pcb(struct pcb_struct * pcb_ptr);

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
error_t suspend_pcb(struct pcb_struct * pcb_ptr);

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
error_t resume_pcb(struct pcb_struct * pcb_ptr);

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
error_t set_pcb_priority(struct pcb_struct * pcb_ptr, const unsigned char pPriority);

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
error_t show_pcb(struct pcb_struct * pcb_ptr);

/**
 * @name show_all_processes
 * @brief Displays all of the processes and their attributes.
 *
 * @return   VOID.
 */
void show_all_processes();

/**
 * @name show_ready_processes
 * @brief Displays all of the ready processes and their attributes.
 *
 * @return  VOID.
 */
void show_ready_processes();

/**
 * @name show_blocked_processes
 * @brief displays all blocked processes and their attributes
 *
 * @return  VOID.
 */
void show_blocked_processes();

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
error_t block_pcb(struct pcb_struct * pcb_ptr);

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
error_t unblock_pcb(struct pcb_struct * pcb_ptr);

#endif
