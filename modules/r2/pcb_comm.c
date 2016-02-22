/**
 * @file pcb_comm.c
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The main functions that manipulate the PCB.
 * @version R2
 *
 */

#include "pcb_comm.h"
#include "pcb.h"
#include <string.h>
#include "../errno.h"
#include "../mpx_supt.h"

int set_pcb_priority_main(int argc, char ** argv){
 if (argc > 3 || argc <2)
 {
  printf("ERROR: Incorrect number of arguments. Please refer to \"setpriority --help\"\n");
 }
 else if(argc == 2)
 {
  if(strcmp(argv[2], "--help") == 0)
  {
   print_help(SETPRIORITY);
  }
 }
 else if(argc == 3)
 {
  unsigned char priority;
  char * name;
  struct pcb_struct * p;
  error_t err = E_NOERROR;
  
  if (strlen(argv[2]) > 10)
  {
   printf("ERROR: Name of process must be within 10 characters.");
   return 0;
  }
  name = argv[2];
  
  if(strlen(argv[3]) == 2)
  {
   if(is_digit(argv[3])){
    priority = atoi(argv[3]);
   }
   else
   {
     printf("ERROR: Priority must be a digit between the values 0 and 9.");
     return 0;
   }
   if(priority > 9)
   {
     printf("ERROR: Priority must be a digit between the values 0 and 9.");
     return 0;
   }
  }
  else
  {
    printf("ERROR: Priority must be a digit between the values 0 and 9.");
    return 0;
  }
  
  if((p = find_pcb(name)) == NULL)
  {
   printf("Priority not set: No process by this name could be found.");
   return 0;
  }
  else
  {
   err = set_pcb_priority(p,priority);
  }
  
  switch(err)
  {
   case E_NOERROR: 
    printf("Priority successfully set for process %s.", name);
   break;
   case E_INVUSRI: 
    printf("ERROR: Priority must be a digit between the values 0 and 9.");
   break;
   case E_PROGERR: 
    printf("PROGERROR: The value used was not the value given");
  }
  
 }
 
 return 0;
}

static int is_digit(char * ch)
{
 return '0' <= ch && ch <= '9';
}

int create_pcb_main(int argc, char ** argv)
{
 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   
  }
  else
  {
   
  }
 }
 else if(argc == 5)
 {
  if(strlen(argv[2]) >= 10)
  {
   
  }
  else if(strcmp(argv[3], "application") && strcmp(argv[3], "system"))
  {
   
  }
  else if(!(strlen(argv[4]) == 1 && is_digit(argv[4])))
  {
   
  }
  else
  {
   enum process_class pClass = strcmp(argv[3], "application") ? system : application;
   
  }
 }
 else
 {
  
 }
 return 0;
}