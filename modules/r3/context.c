
#include "context.h"
#include "../mpx_supt.h"
/**
 *  Context struct 
 */
 struct context {
     u32int gs, fs, es, ds;
     u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
     u32int eip, cs, eflags;
 };