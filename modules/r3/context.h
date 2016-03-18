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

extern struct context* old_context;

/**
 *  Context struct 
 */
 
struct context {
    u32int gs, fs, es, ds;
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32int eip, cs, eflags;
};
 
/**
 * @name sys_call
 * @brief system call interrupt
 *
 * @param   context* registers  current registers
 *
 * @return   result      if there is no current process running, it will load new context. If the process is still running, it will load its old context.
 */ 
 
u32int * sys_call(struct context* registers);

/**
 * @name load_process
 * @brief loads a process into the PCB.
 *
 * @param  pName        Process Name
 * @param  pClass       Process Class
 * @param  pPriority    Process Priority
 * @param  *function()  A function pointer
 *
 * @return  new_pcb     Returns the values of the new PCB
 */

struct pcb_struct * load_process(const char * pName, const enum process_class pClass, const unsigned char pPriority, void (*function)());

/**
 * @name yield_main
 * @brief Requests an IDLE interrupt.
 *
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 *
 * @return  0
 */

int yield_main(int argc, char ** argv);

/**
 * @name load_r3_main
 * @brief Loads the main function of R3.
 *
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 *
 * @return  0
 */

int load_r3_main(int argc, char ** argv);

#endif