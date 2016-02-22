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

void pcb_init();

struct pcb_struct * allocate_pcb();

error_t free_pcb(struct pcb_struct * pcb_ptr);

struct pcb_struct * setup_pcb(const char * pName, const enum process_class pClass, const unsigned char pPriority);

struct pcb_struct * find_pcb(const char * pName);

error_t insert_pcb(struct pcb_struct * pcb_ptr);

error_t remove_pcb(struct pcb_struct * pcb_ptr);

error_t suspend_pcb(struct pcb_struct * pcb_ptr);

error_t resume_pcb(struct pcb_struct * pcb_ptr);

error_t set_pcb_priority(struct pcb_struct * pcb_ptr, const unsigned char pPriority);

error_t show_pcb(const char * pName);

void show_all_processes();

void show_ready_processes();

void show_blocked_processes();

error_t block_pcb(struct pcb_struct * pcb_ptr);

error_t unblock_pcb(struct pcb_struct * pcb_ptr);

#endif
