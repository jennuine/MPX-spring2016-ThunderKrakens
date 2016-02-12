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
#include "string.h"

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
 * @name insert_pcb
 * @brief Inserts PCB into the appropriate queue.
 *
 * @param pcb_ptr 	The PCB pointer.
 * @return E_INSERT_PCB	The PCB was inserted.
 *
 */


error_t insert_pcb (struct pcb_struct * pcb_ptr)
{
    struct pcb_queue_node * current;
    
    //double check how to check enum equals -> It is the other way around
    if (pcb -> running_state == process_state.ready)
    {
        current = ready_queue.head;
        
        while (current != ready_queue->tail)
        {
            
                if (current->actual_pcb->priority <= pcb->priority
		 && pcb->priority < current->next->actual_pcb->priority)
                {
                    current->next = pcb->next; 
                    current->prev = pcb; 
            
                    pcb->next = current;
                    pcb->next->prev = current;
                    
		    return E_INSERT_PCB;
                    break;
                }
            
            current = current->next;
        }
    }

	return E_INSERT_PCB_FAIL;
}

/**
 * @name remove_pcb
 * @brief Removes PCB from the queue it is currently in.
 *
 * @param pcb_ptr 	The PCB pointer.
 * @return E_REMOVE_PCB 	PCB removed
 * @return E_REMOVE_PCB_FAIL	Failed to remove PCB
 *
 */

error_t remove_pcb(struct pcb_struct * pcb_ptr)
{
	struct pcb * current = find_pcb();

	if (pcb_ptr == NULL )
	{
  	return E_NOT_FOUND;
	}

	current->next->prev = current->prev;
	current->prev->next = current->next;
	free_pcb(current);

 	return E_REMOVE_PCB;

}





