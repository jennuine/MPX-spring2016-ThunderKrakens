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
    struct context *begin_address; /**< Beginning address */
    u32int size; /**< Indicates size of block in bytes */
    char *pcb_name[SIZE_OF_PCB_NAME]; /**< Name of PCB process that is housed in block */
    struct cmcb *prev, *next; /**< Links to previous and next blocks of same type */
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
    u32int size; /**< Amount of free space or space required to hold item */
};

#endif