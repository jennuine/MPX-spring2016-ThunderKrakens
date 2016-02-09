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

static struct pcb_queue ReadQueue;
static struct pcb_queue BlockedQueue;

void pcb_init()
{
  ReadQueue.count = 0;
  ReadQueue.head = NULL;
  ReadQueue.tail = NULL;

  BlockedQueue.count = 0;
  BlockedQueue.head = NULL;
  BlockedQueue.tail = NULL;
}
