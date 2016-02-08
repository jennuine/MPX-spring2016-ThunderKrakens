/**
 * @file pcb.c
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The Process Control Block
 * @version R2
 *
 */

#include "pcb.h"

#define NULL 0;

static pcb_queues ReadQueue;
static pcb_queues BlockedQueue;

void pcb_init()
{
  ReadQueue.count = 0;
  ReadQueue.head = NULL;
  ReadQueue.tail = NULL;

  BlockedQueue.count = 0;
  BlockedQueue.head = NULL;
  BlockedQueue.tail = NULL;
}
