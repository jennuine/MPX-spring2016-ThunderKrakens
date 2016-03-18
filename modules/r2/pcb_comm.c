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
#include "../r1/r1.h"

static int is_digit(char ch)
{
 return '0' <= ch && ch <= '9';
}

/**
 * @name suspend_pcb_main.
 *
 * @brief The main function for the "suspend PCB".
 *    Accepted formats: pcb suspend <name>
 *                      pcb suspend --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int suspend_pcb_main(int argc, char ** argv)
{
 struct pcb_struct * pcb_ptr = NULL;

 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(SUSPDPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %d!\n", SIZE_OF_PCB_NAME);
  }
  else if((pcb_ptr = find_pcb(argv[2])) == NULL)
  {
   printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
  }
  else
  {
   error_t err = suspend_pcb(pcb_ptr);
   switch(err)
   {
    case E_NOERROR:
     printf("The process had been suspended successfully!\n");
    break;
    case E_NULL_PTR:
     printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
    break;
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name resume_pcb_main.
 *
 * @brief The main function for the "resume PCB".
 *    Accepted formats: pcb resume <name>
 *                      pcb resume --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int resume_pcb_main(int argc, char ** argv)
{
 struct pcb_struct * pcb_ptr = NULL;

 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(RESUMEPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %!\n", SIZE_OF_PCB_NAME);
  }
  else if((pcb_ptr = find_pcb(argv[2])) == NULL)
  {
   printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
  }
  else
  {
   error_t err = resume_pcb(pcb_ptr);
   switch(err)
   {
    case E_NOERROR:
     printf("The process had been resumed successfully!\n");
    break;
    case E_NULL_PTR:
     printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
    break;
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name set_pcb_priority_main.
 *
 * @brief The main function for the "set PCB priority".
 *    Accepted formats: pcb setpriority <name> <priority>
 *                      pcb setpriority --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int set_pcb_priority_main(int argc, char ** argv){
 if (argc > 4 || argc < 4)
 {
  printf("ERROR: Incorrect number of arguments. Please refer to \"setpriority --help\"\n");
 }
 else if(argc == 2 && !strcmp(argv[2], "--help"))
 {
   print_help(SETPCBPRIO);
 }
 else if(argc == 4)
 {
  unsigned char priority;
  char * name;
  struct pcb_struct * p;
  error_t err = E_NOERROR;

  if (strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: Name of process must be within %d and more than 0 characters.\n", SIZE_OF_PCB_NAME);
   return 0;
  }
  name = argv[2];

  if(strlen(argv[3]) == 1)
  {
   if(is_digit(*argv[3])){
    priority = atoi(argv[3]);
   }
   else
   {
     printf("ERROR: Priority must be a digit between the values 0 and 9.\n");
     return 0;
   }
  }
  else
  {
    printf("ERROR: Priority must be a digit between the values 0 and 9.\n");
    return 0;
  }

  if((p = find_pcb(name)) == NULL)
  {
   printf("Priority not set: No process by this name could be found.\n");
   return 0;
  }
  else
  {
   err = set_pcb_priority(p,priority);
  }

  switch(err)
  {
   case E_NOERROR:
    printf("Priority successfully set for process %s.\n", name);
   break;
   case E_INVPARA:
    printf("ERROR: The arguments you provided are not valid, please check them again!\n");
   break;
  }

 }

 return 0;
}

/**
 * @name show_pcb_main.
 *
 * @brief The main function for the "Show PCB", "Show all Processes", "Show Ready Processes", and "Show Blocked Processes".
 *    Accepted formats: pcb show -name [name]
 *                      pcb show -all
 *                      pcb show -ready
 *                      pcb show -blocked
 *                      pcb show --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int show_pcb_main(int argc, char ** argv)
{

 if (argc > 4 || argc < 3 )
 {
  printf("ERROR: Incorrect number of arguments. Please refer to \"pcb show --help\"\n");
  return 0;
 }
 else if(argc == 3)
 {

  if (!strcmp(argv[2], "--help"))
  {
   print_help(SHOWPCB);
  }
  else if (!strcmp(argv[2], "-all"))
  {
   show_all_processes();
  }
  else if (!strcmp(argv[2], "-ready"))
  {
   show_ready_processes();
  }
  else if (!strcmp(argv[2], "-blocked"))
  {
   show_blocked_processes();
  }
  else if (!strcmp(argv[2], "-name"))
  {
   printf("ERROR: Too few arguments for the \"-name\"! \n\n");
  }
  else
  {
   printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
  }
 }
 else if(argc == 4)
 {
  if (!strcmp(argv[2], "--help"))
  {
   printf("ERROR: Too many arguments for the \"--help\"! \n\n");
  }
  else if (!strcmp(argv[2], "-all"))
  {
   printf("ERROR: Too many arguments for the \"-all\"! \n\n");
  }
  else if (!strcmp(argv[2], "-ready"))
  {
   printf("ERROR: Too many arguments for the \"-ready\"! \n\n");
  }
  else if (!strcmp(argv[2], "-blocked"))
  {
   printf("ERROR: Too many arguments for the \"-blocked\"! \n\n");
  }
  else if (!strcmp(argv[2], "-name"))
  {
   struct pcb_struct * pcb_ptr = NULL;

   if((pcb_ptr = find_pcb(argv[3])) == NULL)
   {
    printf("ERROR: Could not find PCB named \'%s\'.\n\n", argv[3]);
   }
   else
   {
    show_pcb(pcb_ptr);
   }
  }
  else
  {
   printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
  }
 }
 return 0;
}

#if 0

/**
 * @name create_pcb_main.
 *
 * @brief The main function for the "Create PCB".
 *    Accepted formats: pcb create <name> <type> <priority>
 *                      pcb create --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int create_pcb_main(int argc, char ** argv)
{
 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(CREATEPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 5)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %d!\n", SIZE_OF_PCB_NAME);
  }
  else if(find_pcb(argv[2]))
  {
   printf("ERROR: The name of the PCB must be unique!\n");
  }
  else if(strcmp(argv[3], "application") && strcmp(argv[3], "system"))
  {
   printf("ERROR: The class of the process can only be \"system\" or \"application\"!\n");
  }
  else if(!(strlen(argv[4]) == 1 && is_digit(*argv[4])))
  {
   printf("ERROR: The priority must be a single digit number with range [0, 9]!\n");
  }
  else
  {
   enum process_class pClass = strcmp(argv[3], "application") ? pcb_class_sys : pcb_class_app;
   unsigned char pPriority = *argv[4] - '0';

   struct pcb_struct * created_pcb = setup_pcb(argv[2], pClass, pPriority);
   error_t err = insert_pcb(created_pcb);
   switch(err)
   {
    case E_NOERROR:
     printf("Process had been successfully created!\n");
    break;
    case E_INVPARA:
    case E_NULL_PTR:
     printf("ERROR: The arguments you provided are not valid, please check them again!\n");
    break;
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name delete_pcb_main.
 *
 * @brief The main function for the "Delete PCB".
 *    Accepted formats: pcb del <name>
 *                      pcb del --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int delete_pcb_main(int argc, char ** argv)
{
 struct pcb_struct * pcb_ptr = NULL;

 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(DELPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %d!\n", SIZE_OF_PCB_NAME);
  }
  else if((pcb_ptr = find_pcb(argv[2])) == NULL)
  {
   printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
  }
  else
  {
    error_t err = remove_pcb(pcb_ptr);
    switch(err)
    {
    case E_INVPARA:
     printf("ERROR: The PCB you choice cannot be removed now.\n");
    break;
    case E_NULL_PTR:
     printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
    break;
    }
    if(err == E_NOERROR && (err = free_pcb(pcb_ptr)) == E_NOERROR)
    {
      printf("The process had been deleted successfully!\n");
    }
    else if(err == E_FREEMEM)
    {
      printf("The process had been deleted successfully!\n\
            \t However, The memory space cannot be actually free now,\n\
            \t since the student_free had not been implemented yet.\n");
    }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name block_pcb_main.
 *
 * @brief The main function for the "block PCB".
 *    Accepted formats: pcb block <name>
 *                      pcb block --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int block_pcb_main(int argc, char ** argv)
{
 struct pcb_struct * pcb_ptr = NULL;

 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(BLOCKPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %d!\n", SIZE_OF_PCB_NAME);
  }
  else if((pcb_ptr = find_pcb(argv[2])) == NULL)
  {
   printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
  }
  else
  {
   error_t err = block_pcb(pcb_ptr);
   switch(err)
   {
    case E_NOERROR:
     printf("The process had been blocked successfully!\n");
    break;
    case E_INVPARA:
     printf("ERROR: The PCB you choice cannot be blocked now.\n");
    break;
    case E_NULL_PTR:
     printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
    break;
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;
}

/**
 * @name unblock_pcb_main.
 *
 * @brief The main function for the "unblock PCB".
 *    Accepted formats: pcb unblock <name>
 *                      pcb unblock --help
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int unblock_pcb_main(int argc, char ** argv)
{
 struct pcb_struct * pcb_ptr = NULL;

 if(argc > 2 && !strcmp(argv[2], "--help"))
 {
  if(argc <= 3)
  {
   print_help(UNBLKPCB);
  }
  else
  {
   printf("ERROR: Too many arguments for \"--help\" option!\n");
  }
 }
 else if(argc == 3)
 {
  if(strlen(argv[2]) >= SIZE_OF_PCB_NAME || strlen(argv[2]) == 0)
  {
   printf("ERROR: The length of the name must be greater than zero and smaller than %d!\n", SIZE_OF_PCB_NAME);
  }
  else if((pcb_ptr = find_pcb(argv[2])) == NULL)
  {
   printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
  }
  else
  {
   error_t err = unblock_pcb(pcb_ptr);
   switch(err)
   {
    case E_NOERROR:
     printf("The process had been unblocked successfully!\n");
    break;
    case E_INVPARA:
     printf("ERROR: The PCB you choice cannot be unblocked now.\n");
    break;
    case E_NULL_PTR:
     printf("ERROR: cannot find the PCB with name \"%s\"\n", argv[2]);
    break;
   }
  }
 }
 else
 {
  printf("ERROR: Invalid arguments you had input. Please refers to \"--help\"!\n");
 }
 return 0;

}

#endif