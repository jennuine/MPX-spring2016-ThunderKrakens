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
#define APP_PROCESS 10;
/* @brief Represents the process class of system process. */
#define SYS_PROCESS 20;
/* @brief The defualt size of the stack for the PCB */
#define SIZE_OF_STACK 1024;

enum process_state
{
  running,
  ready,
  blocked
};

struct pcb_struct;
struct pcb_queue;
struct pcb_queue_node;

struct pcb_struct
{
  char name[10];
  unsigned char class;
  unsigned char priority;
  enum process_state running_state;
  unsigned char is_suspended;
  unsigned char * stack_top;
  unsigned char * stack_base;
  struct pcb_queue * other_pcb;
};

struct pcb_queue_node
{
  struct pcb_queue_node * prev;
  struct pcb_struct actual_pcb;
  struct pcb_queue_node * next;
};

struct pcb_queue
{
  int count;
  struct pcb_queue_node * head;
  struct pcb_queue_node * tail;
};

void pcb_init();

struct pcb_struct * allocate_pcb();

error_t free_pcb(struct pcb_struct * pcb_ptr);

struct pcb_struct * setup_pcb(const char * pName, const unsigned char pClass, const unsigned char pPriority);

struct pcb_struct * find_pcb(const char * pName);

error_t insert_pcb(struct pcb_struct * pcb_ptr);

error_t remove_pcb(struct pcb_struct * pcb_ptr);

error_t suspend_pcb(struct pcb_struct * pcb_ptr);

error_t resume_pcb(struct pcb_struct * pcb_ptr);

error_t set_pcb_priority(struct pcb_struct * pcb_ptr, const unsigned char pPriority);

error_t show_pcb(struct pcb_struct * pcb_ptr);

void show_all_processes();

void show_ready_processes();

void show_blocked_processes();

error_t block_pcb(struct pcb_struct * pcb_ptr);

error_t unblock_pcb(struct pcb_struct * pcb_ptr);

#endif