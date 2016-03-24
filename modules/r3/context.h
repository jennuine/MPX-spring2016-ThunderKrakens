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
extern struct pcb_struct *cop;

/**
 * Context structure that holds the 15 CPU register values to begin and resume process execution 
 */
struct context {
    u32int gs, /**< Segment register */
        fs, /**< Segment register */
        es, /**< Segment register */
        ds; /**< Segment register */
    u32int edi,  /**< General-purpose register */
        esi, /**< General-purpose register */ 
        ebp, /**< General-purpose register */
        esp, /**< General-purpose register */ 
        ebx, /**< General-purpose register */
        edx, /**< General-purpose register */
        ecx, /**< General-purpose register */
        eax; /**< General-purpose register */
    u32int eip,  /**< Status and control register */
        cs,  /**< Status and control register */
        eflags; /**< Status and control register */
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