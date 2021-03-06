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

/** The maximum heap size */
#define MAX_HEAP_SIZE 5000

/** Global variable labeling start of memory */
extern u32int start_of_memory;

/**
 * @brief The MCB structure.
 * MCB members are private to prevent potential manipulation so that 
 * the MPX system can remain functioning correctly.
 */
 struct mcb;

/**
 * @name init_heap
 * @brief Allocates all the memory for MPX.
 *
 * @param  size  Size of heap in bytes.
 */
void init_heap(u32int size);

/**
 * @name mcb_allocate
 * @brief Allocates a memory block.
 *
 * @param  mem_size     The MCB size to be allocated.
 * 
 *
 * @return  Address to allocated MCB.
 * @return  NULL if not enough space in free memory found.
 */
void * mcb_allocate(u32int mem_size);

/**
 * @name show_mcb
 * @brief Displays the allocated or free memory block's address, 
 *      previous and next pointers, and block's size.
 * 
 * @param  mcb_ptr  MCB Pointer
 */
void show_mcb(struct mcb * mcb_ptr);

/**
 * @name show_free_mcb
 * @brief Displays all the free memory.
 */
void show_free_mcb();

/**
 * @name show_allocated_mcb
 * @brief Displays all the allocated MCBs.
 */
void show_allocated_mcb();

/**
 * @name show_all_mcb
 * @brief Displays all the free and allocated memory.
 */
void show_all_mcb();

/**
 * @name is_mcb_empty
 * @brief Checks if the heap is empty.
 * 
 * @return 0 or 1 (true or false).
 */
int is_mcb_empty();

/**
 * @name mcb_free_mpx
 * @brief Calls @ref mcb_free to free memory block, used as parameter for @ref sys_set_free in kmain.c.
 *
 * @param  mem_ptr    Memory Pointer
 * 
 * @return 0 when finished with exectuion.
 */
int mcb_free_mpx(void * mem_ptr);

/**
 * @name mcb_allocate_mpx
 * @brief Calls @ref mcb_allocate to allocate memory block, used as parameter for @ref sys_set_malloc in kmain.c.
 *
 * @param  size     Size of block in bytes to allocate.
 * 
 * @return Address of allocated MCB.
 */
u32int mcb_allocate_mpx(u32int size);

/**
 * @name mcb_allocate_mpx2
 * @brief MCB allocate MPX.
 * 
 * @param mem_size  Block size to allocate.
 * @param name  Name of the pcb process.
 * @return  Address pointer to allocated memory only used for testing in commhand for module R5.
 */
void *mcb_allocate_mpx2(u32int size, const char *name);

//Permanent User's Commands

/**
 * @name show_mcb_main.
 * @brief The function of show MCB for commhand.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int show_mcb_main(int argc, char ** argv);

/**
 * @name shutdown_mcb.
 * @brief Shutdown the pcb during the shutdown procedure.
 * 
 * @return  0 when finished with execution.
 */
void shutdown_mcb();

#if WITH_R5_TEMP_CMD
//Temperary User's Commands

/**
 * @name init_heap_main
 * @brief The main function of initate heap only used for testing in commhand for module R5.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int init_heap_main(int argc, char ** argv);

/**
 * @name mcb_allocate_main.
 * @brief The main function of MCB allocate only used for testing in commhand for module R5.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int mcb_allocate_main(int argc, char ** argv);

/**
 * @name mcb_free_main.
 * @brief The main function of free MCB only used for testing in commhand for module R5.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int mcb_free_main(int argc, char ** argv);

/**
 * @name is_mcb_empty_main.
 * @brief The main function of is MCB empty only used for testing in commhand for module R5.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int is_mcb_empty_main(int argc, char ** argv);
#endif

#endif