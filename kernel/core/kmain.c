/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/r1/r1.h"
#include "modules/r2/pcb.h"
#include "modules/r3/context.h"
#include "modules/r5/mcb.h"

#define HEAP_SIZE 50000

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

   // 0) Initialize Serial I/O and call mpx_init
   klogv("Starting MPX boot sequence...");

   init_serial(COM1);
   set_serial_in(COM1);
   set_serial_out(COM1);

   mpx_init(MODULE_R5);

   klogv("Initialized serial I/O on COM1 device...");

   // 1) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.

   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }

   // 2) Descriptor Tables
   klogv("Initializing descriptor tables...");

   init_gdt();
   init_idt();

   init_pic();
   init_irq();
   sti();

   // 4) Virtual Memory
   klogv("Initializing virtual memory...");
   init_paging();
   
   klogv("Initializing heap and memory control blocks...");
   sys_set_malloc(&mcb_allocate_mpx);
   sys_set_free(&mcb_free_mpx);
   init_heap(HEAP_SIZE);

   
	klogv("Initializing PCB queues...");
	pcb_init();
	
   // 5) Call Commhand
   //klogv("Transferring control to commhand...");
   //commhand();
   
   resume_pcb(load_process(COMMHAND_PCB_NAME, pcb_class_sys, 0, &commhand));
   resume_pcb(load_process(IDLE_PCB_NAME, pcb_class_sys, 0, &idle));
   sys_req(IDLE);

   // 11) System Shutdown
   klogv("Starting system shutdown procedure...");
   klogv("\t shutdown PCB...");
   shutdown_pcb();
   klogv("\t shutdown MCB...");
   shutdown_mcb();

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
