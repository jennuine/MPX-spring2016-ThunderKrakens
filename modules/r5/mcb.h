/**
 * @file mcb.h
 * @author Thunder Krakens
 * @date April 8th, 2016
 * @brief Memory Control Block
 * @version R5
 *
 */

#ifndef _MCB_H
#define _MCB_H

#include <system.h>
#include "../r2/pcb.h"

#define MAX_HEAP_SIZE 5000

/** Global variable labeling start of memory */
extern u32int start_of_memory;

/**
* PCB process class types.
*/
enum mcb_type
{
  free, /**< Process is an application process. */
  allocated /**< Process is a system process. */
} __attribute__ ((packed));


/**
 * Complete Memory Control Block Struct
 */
struct cmcb {
    enum mcb_type type; /**< Type indicating free or allocated */
    void * begin_address; /**< Beginning address */
    u32int size; /**< Indicates size of block in bytes */
    //char *pcb_name[SIZE_OF_PCB_NAME]; /**< Name of PCB process that is housed in block */
    // struct mcb *prev, *next; /**< Links to previous and next blocks of same type */
};

/**
 * Limited Memory Control Block Struct
 */
struct lmcb {
    enum mcb_type type; /**< Type indicating free or allocated */
    u32int size; /**< Indicates size of block in bytes */
};

/**
 * Memory Control Block Struct
 */
struct mcb {
    struct cmcb * complete_mcb; /**< Complete Memory Control Block */
    struct lmcb * limited_mcb; /**< Limited Memory Control Block */
    // u32int size; /**< Amount of free space or space required to hold item */
    struct mcb *prev, *next;
};

void init_heap(u32int size);

void * mcb_allocate(u32int mem_size);

error_t mcb_free(void * mem_ptr);

void show_mcb(struct mcb * mcb_ptr);

void show_free_mcb();

void show_allocated_mcb();

void show_all_mcb();

int is_mcb_empty();

//Permanent User's Commands
int show_mcb_main(int argc, char ** argv);

//Temperary User's Commands
int init_heap_main(int argc, char ** argv);

int mcb_allocate_main(int argc, char ** argv);

int mcb_free_main(int argc, char ** argv);

int is_mcb_empty_main(int argc, char ** argv);

#endif