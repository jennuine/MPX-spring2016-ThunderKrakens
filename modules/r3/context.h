
/**
 * @file context.h
 * @author Thunder Krakens
 * @date March 18th, 2016
 * @brief Context Switching
 * @version R3
 *
 */

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <system.h>
#include "../r2/pcb.h"
#include "../errno.h"

/**
 *  Context struct 
 */
struct context {
    u32int gs, fs, es, ds;
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32int eip, cs, eflags;
};
 
u32int * sys_call(struct context* registers);

error_t load_process(const char * pName, const enum process_class pClass, const unsigned char pPriority, void (*function)());

int yield_main(int argc, char ** argv);

int load_r3_main(int argc, char ** argv);

#endif