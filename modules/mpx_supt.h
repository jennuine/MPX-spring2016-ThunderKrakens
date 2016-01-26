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


typedef struct {
  int op_code;
  int device_id;
} param;

/*
  Procedure..: sys_req
  Description..: Generate interrupt 60H
  Params..: int op_code (IDLE)
*/
int sys_req( int op_code );

/*
  Procedure..: mpx_init
  Description..: Initialize MPX support software
  Params..: int cur_mod (symbolic constants MODULE_R1, MODULE_R2, etc
*/
void mpx_init(int cur_mod);

/*
  Procedure..: set_malloc
  Description..: Sets the memory allocation function for sys_alloc_mem
  Params..: Function pointer
*/
void sys_set_malloc(u32int (*func)(u32int));

/*
  Procedure..: set_free
  Description..: Sets the memory free function for sys_free_mem
  Params..: s1-destination, s2-source
*/
void sys_set_free(int (*func)(void *));

/*
  Procedure..: sys_alloc_mem
  Description..: Allocates a block of memory (similar to malloc)
  Params..: Number of bytes to allocate
*/
void *sys_alloc_mem(u32int size);

/*
  Procedure..: sys_free_mem
  Description..: Frees memory
  Params..: Pointer to block of memory to free
*/
int sys_free_mem(void *ptr);

/*
  Procedure..: idle
  Description..: The idle process
  Params..: None
*/
void idle();

#endif
