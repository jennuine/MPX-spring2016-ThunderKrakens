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

static struct pcb_queue read_queue;
static struct pcb_queue blocked_queue;

void pcb_init()
{
  read_queue.count = 0;
  read_queue.head = NULL;
  read_queue.tail = NULL;

  blocked_queue.count = 0;
  blocked_queue.head = NULL;
  blocked_queue.tail = NULL;
}
