/**
 * @file mpx_supt.h
 * @author Thunder Krakens
 * @date March 18, 2016
 * @brief MPX System Supplementaries
 * @version R3
 *
 */

#ifndef _MPX_SUPT_H
#define _MPX_SUPT_H

#include <system.h>

#define EXIT 0
#define IDLE 1
#define READ 2
#define WRITE 3

#define MODULE_R1 0
#define MODULE_R2 1
#define MODULE_R3 2
#define MODULE_R4 4
#define MODULE_R5 8

/**
 * A structure to represent interrupt
 */
typedef struct {
  int op_code; /**< interrupt's operation */
  int device_id; /**< interrupt's device */
} param;

/**
* @name sys_req
* @brief Generate interrupt 60H
* @param int op_code (IDLE)
*/
int sys_req( int op_code );

/**
* @name mpx_init
* @brief Initialize MPX support software
* @param int cur_mod (symbolic constants MODULE_R1, MODULE_R2, etc
*/
void mpx_init(int cur_mod);

/**
* @name set_malloc
* @brief Sets the memory allocation function for sys_alloc_mem
* @param Function pointer
*/
void sys_set_malloc(u32int (*func)(u32int));

/**
* @name set_free
* @brief Sets the memory free function for sys_free_mem
* @param s1-destination, s2-source
*/
void sys_set_free(int (*func)(void *));

/**
* @name sys_alloc_mem
* @brief Allocates a block of memory (similar to malloc)
* @param Number of bytes to allocate
*/
void *sys_alloc_mem(u32int size);

/**
* @name sys_free_mem
* @brief Frees memory
* @param Pointer to block of memory to free
*/
int sys_free_mem(void *ptr);

/**
* @name idle
* @brief The idle process
* @param None
*/
void idle();

/**
* @name get_op_code
* @brief Returns the interrupt's operation code
* @param None
*/
int get_op_code();

#endif
