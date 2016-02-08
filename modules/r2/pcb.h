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

enum process_running_stat
{
  PStatRunning,
  PStatReady,
  PStatBlocked
};

typedef struct
{
  char pName[10];
  unsigned char pClass;
  unsigned char pPriority;
  enum process_running_stat pRunningStat;
  unsigned char pIsSuspended;
  unsigned char* pStackTop;
  unsigned char* pStackBase;
  void* pOtherPcb;
} pcb_struct;

typedef struct
{
  void* priv;
  pcb_struct ActualPcb;
  void* next;
} pcb_node;

typedef struct
{
  int count;
  pcb_node * head;
  pcb_node * tail;
} pcb_queues;

void pcb_init();

pcb_struct * allocate_pcb();

error_t free_pcb(pcb_struct * PcbPtr);

pcb_struct * setup_pcb(const char * pName, const unsigned char pClass, const unsigned char pPriority);

pcb_struct * find_pcb(const char * pName);

error_t insert_pcb(pcb_struct * PcbPtr);

error_t remove_pcb(pcb_struct * PcbPtr);

error_t suspend_pcb(pcb_struct * PcbPtr);

error_t resume_pcb(pcb_struct * PcbPtr);

error_t set_pcb_priority(pcb_struct * PcbPtr, const unsigned char pPriority);

error_t show_pcb(pcb_struct * PcbPtr);

void show_all_processes();

void show_ready_processes();

void show_blocked_processes();

error_t block_pcb(pcb_struct * PcbPtr);

error_t unblock_pcb(pcb_struct * PcbPtr);

#endif
