
#include <string.h>
#include "context.h"
#include "../mpx_supt.h"
#include "../r1/r1.h"
#include "procsr3.c"

static struct pcb_struct * cop = NULL;
static struct context* old_context = NULL;

static int is_digit(char ch)
{
 return '0' <= ch && ch <= '9';
}

u32int* sys_call(struct context* registers)
{
 if(cop)
 { // sys_call had been called.
  if(get_op_code() == IDLE)
  {
   //If params.op code == IDLE, save the context
   //printf("*Debug* save: @ %X.\n", registers->eip);
   save_running_process(cop, registers);
   cop = NULL;
  }
  else if(get_op_code() == EXIT)
  {
   free_pcb(cop);
   cop = NULL;
  }
 }
 else
 { // sys_call had not been called, save a reference to old (the caller's) context in a global variable.
  old_context = registers;
 }
 
 u32int * result = NULL;
 cop = get_running_process();
 if(cop != NULL)
 {
  result = (u32int*)get_stack_top(cop);
 }
 else
 {
  result = (u32int*)old_context;
 }
   //printf("*Debug* return: @ %X.\n", ((struct context*)result)->eip);
 
 return result;
}

error_t load_process(const char * pName, const enum process_class pClass, const unsigned char pPriority, void (*function)())
{
 error_t result = E_NOERROR;
 
 struct pcb_struct * new_pcb = setup_pcb( pName , pClass , pPriority );
 if(new_pcb)
 {
  suspend_pcb(new_pcb);
  struct context * cp = (struct context *)(get_stack_top(new_pcb));
  memset (cp , 0, sizeof(struct context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(get_stack_base(new_pcb));
  cp->esp = (u32int)(get_stack_top(new_pcb));
  cp->eip = (u32int)function;// The function correlating to the process , ie. proc1
  //printf("*Debug* load: %s, @ %X <= %X.\n", pName, cp->eip, function);
  cp->eflags = 0x202;
  
  result = insert_pcb(new_pcb);
 }
 else
 {
  result = E_INVPARA;
 }
 
 return result;
}


int yield_main(int argc, char ** argv)
{
 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(YIELD);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 2)
 {
  asm volatile ("int $60");
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

int load_r3_main(int argc, char ** argv)
{
 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(LOADR3);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(!(strlen(argv[2]) == 1 && is_digit(*argv[2])))
  {
   printf("ERROR: The priority must be a single digit number with range [0, 9]!\n");
  }
  else
  {
   error_t error_code = E_NOERROR;
   if(error_code == E_NOERROR) 
    error_code = load_process("proc1", pcb_class_app, 1, &proc1);
   if(error_code == E_NOERROR) 
    error_code = load_process("proc2", pcb_class_app, 1, &proc2);
   if(error_code == E_NOERROR) 
    error_code = load_process("proc3", pcb_class_app, 1, &proc3);
   if(error_code == E_NOERROR) 
    error_code = load_process("proc4", pcb_class_app, 1, &proc4);
   if(error_code == E_NOERROR) 
    error_code = load_process("proc5", pcb_class_app, 1, &proc5);
   
   if(error_code == E_NOERROR)
   {
    printf("Processes loaded successfully!\n");
   }
   else
   {
    printf("Failed to load processes!\n");
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}
