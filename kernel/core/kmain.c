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
#include "modules/R1/R1.h"

int bcdConv(int num);
void settime(const char *time);
void setdate(const char *date);
void help(const char *input);
dateTime gettime();
dateTime getdate();


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

   mpx_init(MODULE_R1);

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

   // 5) Call Commhand
   klogv("Transferring control to commhand...");
   commhand();

   // 11) System Shutdown
   klogv("Starting system shutdown procedure...");

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}

/*
Name: bcdConv
Desc: Converts decimal value to BCD
Para: integer number
*/
int bcdConv(int num)
{
   int num;
   num = ((num/16)*10 + (num%16));
   return num;
}

/*
Name: settime
Desc: Sets the time
Para: time - input by the user in format of hh:mm:ss
*/

void settime(const char *time){
   int length;
   length = strlen(time);
    
   // To check whether the input is in the correct format or not
   if(length != 8)
   {
      serial_print("The inpuit cannot be recognized.");
      serial_print(time);
      serial_println("Incorrect format. Please refer to \"help settime\" for more information.");
   } else {
	
      const char h[3] = {time[0],time[1], '\0'};
      const char m[3] = {time[3],time[4], '\0'};
      const char s[3] = {time[6],time[7], '\0'};
	  int hour, min, sec;
	  hour = atoi(h);
	  min = atoi(m);
	  sec = atoi(s);

      // To check whether the vavlues are within acceptable range
	  if(hour >= 0 && hour <= 23 && min >= 0 && min <= 59 && sec >= 0 && sec <= 59)
      {
	     cli();

         outb(0x70, 0x04);
         outb(0x71, bcd(hour));

         outb(0x70, 0x02);
         outb(0x71, bcd(min));

         outb(0x70, 0x00);
         outb(0x71, bcd(sec));

         sti();
         
		} else {
            serial_print("The inpuit cannot be recognized.");
			serial_print(time);
			serial_println(("Incorrect format. Please refer to \"help settime\" for more information.");
		}
	}
}

/*
Name: gettime
Desc: returns current time
Param: none, its a getter function
*/

dateTime gettime()
{
   dateTime sys_datetime;
   outb(0x70, 0x04);
   sys_datetime.hour = BCDToDecimal(inb(0x71));

   outb(0x70, 0x02);
   sys_datetime.min = BCDToDecimal(inb(0x71));

   outb(0x70, 0x00);
   sys_datetime.sec = BCDToDecimal(inb(0x71));

return sys_datetime;
}