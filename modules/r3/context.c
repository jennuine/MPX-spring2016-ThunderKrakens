/**
 * @file context.c
 * @author Thunder Krakens
 * @date March 18th, 2016
 * @brief Context Switching
 * @version R3
 *
 */

#include <string.h>
#include "context.h"
#include "../mpx_supt.h"
#include "../r1/r1.h"
#include "procsr3.c"

static struct pcb_struct * cop = NULL;
struct context* old_context = NULL;

/**
 * @name is_digit
 * @brief Checks if the character is a digit.
 *
 * @param   ch  character selected.
 *
 * @return a digit between 0 and 9.
 */

static int is_digit(char ch)
{
 return '0' <= ch && ch <= '9';
}

/**
 * @name sys_call
 * @brief system call interrupt
 *
 * @param   context* registers  current registers
 *
 * @return   result      if there is no current process running, it will load new context. If the process is still running, it will load its old context.
 */

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
   // printf("*Debug* save: @ %X.\n", registers->esp);
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

struct pcb_struct * load_process(const char * pName, const enum process_class pClass, const unsigned char pPriority, void (*function)())
{
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
  
  if(insert_pcb(new_pcb) != E_NOERROR)
  {
   free_pcb(new_pcb);
   new_pcb = NULL;
  }
 }
 
 return new_pcb;
}

/**
 * @name yield_main
 * @brief Requests an IDLE interrupt.
 *
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 *
 * @return  0
 */

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
  sys_req(IDLE);
  
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name load_r3_main
 * @brief Loads the main function of R3.
 *
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 *
 * @return  0
 */
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
   unsigned char priority = (unsigned char)atoi(argv[2]);
   struct pcb_struct * new_pcb = load_process("proc1", pcb_class_app, priority, &proc1);
   if(new_pcb)
    printf("Process %s loaded successfully!\n", "proc1");
   else
    printf("Failed to load process %s!\n", "proc1");
    
   if(new_pcb) 
    new_pcb = load_process("proc2", pcb_class_app, priority, &proc2);
   if(new_pcb)
    printf("Process %s loaded successfully!\n", "proc2");
   else
    printf("Failed to load process %s!\n", "proc2");
    
   if(new_pcb) 
    new_pcb = load_process("proc3", pcb_class_app, priority, &proc3);
   if(new_pcb)
    printf("Process %s loaded successfully!\n", "proc3");
   else
    printf("Failed to load process %s!\n", "proc3");
    
   if(new_pcb) 
    new_pcb = load_process("proc4", pcb_class_app, priority, &proc4);
   if(new_pcb)
    printf("Process %s loaded successfully!\n", "proc4");
   else
    printf("Failed to load process %s!\n", "proc4");
    
   if(new_pcb) 
    new_pcb = load_process("proc5", pcb_class_app, priority, &proc5);
   if(new_pcb)
    printf("Process %s loaded successfully!\n", "proc5");
   else
    printf("Failed to load process %s!\n", "proc5");   
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}
