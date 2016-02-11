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
  pcb_ptr.process_suspended = false;
  if (insert_pcb(pcb_ptr) != E_NOERROR){
    return E_INSERT_PCB;
  }

  if (remove_pcb(pcb_ptr) != E_NOERROR){
    return E_REMOVE_PCB;
  } 
  
  return 0;
}

error_t resume_pcb(struct pcb_struct * pcb_ptr){
  pcb_ptr.process_suspended = false;
  if (insert_pcb(pcb_ptr) != E_NOERROR){
    return E_INSERT_PCB;
  }
  if (remove_pcb(pcb_ptr) != E_NOERROR){
    return E_REMOVE_PCB;
  } 
  return 0;
}
