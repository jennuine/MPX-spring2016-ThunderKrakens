/**
 * @file pcb.h
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R3
 *
 */

#ifndef _PCB_H
#define _PCB_H

#include "../errno.h"


/** The defualt size of the stack for the PCB. */
#define SIZE_OF_STACK 1024
/** The max length of the PCB name string. */
#define SIZE_OF_PCB_NAME 10
/** The name of the command handler PCB. */
#define COMMHAND_PCB_NAME "commhand"
/** The name of the idle PCB. */
#define IDLE_PCB_NAME "idle"

/**
* PCB process class types.
*/
enum process_class
{
  pcb_class_app, /**< Process is an application process. */
  pcb_class_sys /**< Process is a system process. */
} __attribute__ ((packed));

/**
 * @brief The PCB structure.
 * PCB members are private to prevent potential manipulation so that 
 * the MPX system can remain functioning correctly.
 */
struct pcb_struct;

/**
 * @brief The PCB queue structure.
 * The PCB queue structure members are private to prevent potential manipulation so that 
 * the MPX system can remain functioning correctly.
 */
struct pcb_queue;

/**
 * The context structure. Please refer to @ref context.h for more information.
 */
struct context;

/**
 * @name  pcb_init
 * @brief Initializes the PCB queues
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
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return  The pointer that point to the PCB structure.
 * @return  NULL if error occured.
 */
struct pcb_struct * setup_pcb(const char * pName, const enum process_class pClass, const unsigned char pPriority);

/**
 * @name find_pcb
 * @brief Will search all queues for a process named pName
 *
 * @param   pName  The char pointer to the desired searched name
 *
 * @return The PCB pointer.
 * @return NULL if PCB is not found
 */
struct pcb_struct * find_pcb(const char * pName);

/**
 * @name insert_pcb
 * @brief Inserts PCB into the appropriate queue.
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 */
error_t show_pcb(struct pcb_struct * pcb_ptr);

/**
 * @name show_all_processes
 * @brief Displays all of the processes and their attributes.
 */
void show_all_processes();

/**
 * @name show_ready_processes
 * @brief Displays all of the ready processes and their attributes.
 */
void show_ready_processes();

/**
 * @name show_blocked_processes
 * @brief Displays all blocked processes and their attributes
 */
void show_blocked_processes();

/**
 * @name block_pcb
 * @brief puts the given pcb into the blocked state and places it into the correct queue
 *
 * @param   pcb_ptr The pointer to the PCB
 *
 * @return The appropiate error code. See @ref errno.h for details.
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
 * @return The appropiate error code. See @ref errno.h for details.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members (By "remove_pcb" or "insert_pcb").
 */
error_t unblock_pcb(struct pcb_struct * pcb_ptr);

/**
 * @name get_running_process
 * @brief gets a unsuspended and unblocked process from the front of the queue, and sets it to running state.
 *
 * @return NULL if there is no process available, otherwise, the pointer that point to the PCB structure.
 */
struct pcb_struct * get_running_process();

/**
 * @name save_running_process
 * @brief sets the running process to ready state, and inserts it to the ready queue.
 *
 * @param   pcb_ptr The pointer to the PCB.
 * @param   new_stack_top The pointer to the new stack top.
 *
 * @return The appropiate error code. See @ref errno.h for details.
 *    Possible error code to be returned:
 *      E_NOERROR   No error.
 *      E_NULL_PTR  Null pointer error.
 *      E_INVPARA   The given PCB has abnormal data members (By "insert_pcb").
 */
error_t save_running_process(struct pcb_struct * pcb_ptr, struct context * new_stack_top);

/**
 * @name get_stack_top
 * @brief gets the pointer to the stack top of the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB.
 *
 * @return The pointer that point to the stack top of the specific PCB.
 * @return NULL if the pcb_ptr is NULL.
 */
unsigned char * get_stack_top(struct pcb_struct * pcb_ptr);

/**
 * @name get_stack_base
 * @brief gets the pointer to the stack base of the specific PCB.
 *
 * @param   pcb_ptr The pointer to the PCB.
 *
 * @return The pointer that point to the stack base of the specific PCB.
 * @return NULL if the pcb_ptr is NULL.
 */
unsigned char * get_stack_base(struct pcb_struct * pcb_ptr);

/**
 * @name shutdown_pcb
 * @brief called when system is going to shutdown, removes all PCBs, free all PCBs.
 */
void shutdown_pcb();

#endif
