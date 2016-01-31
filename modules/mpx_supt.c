#include "mpx_supt.h"
#include <mem/heap.h>

param params;
int current_module = -1;
u32int (*student_malloc)(u32int);
int (*student_free)(void *);

int sys_req( int  op_code )
{
  params.op_code = op_code;
  asm volatile ("int $60");
  return 0;
}

void mpx_init(int cur_mod)
{
  current_module = cur_mod;
}

void sys_set_malloc(u32int (*func)(u32int))
{
  student_malloc = func;
}

void sys_set_free(int (*func)(void *))
{
  student_free = func;
}

void *sys_alloc_mem(u32int size)
{
  if (current_module < MODULE_R5)
    return (void *) kmalloc(size);
  else
    return (void *) (*student_malloc)(size);
}

int sys_free_mem(void *ptr)
{
  if (current_module >= MODULE_R5)
    return (*student_free)(ptr);
  // otherwise we don't free anything
  return -1;
}

void idle()
{
  while(1){
    sys_req(IDLE);
  }
}
