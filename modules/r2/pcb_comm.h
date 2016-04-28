/**
 * @file pcb_comm.h
 * @author Thunder Krakens
 * @date February 7th, 2016
 * @brief The main functions that manipulate the PCB.
 * @version R2
 *
 */

 #ifndef _PCB_COMM_H
 #define _PCB_COMM_H
 
#include "../cmd_orders.h"

 /**
  * @name suspend_pcb_main
  *
  * @brief The main argument handler for the suspend PCB command.
  *    Accepted formats: pcb suspend <name>
  *                      pcb suspend --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int suspend_pcb_main(int argc, char ** argv);

 /**
  * @name resume_pcb_main
  *
  * @brief The main argument handler for the resume PCB command.
  *    Accepted formats: pcb resume <name>
  *                      pcb resume --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int resume_pcb_main(int argc, char ** argv);

 /**
  * @name set_pcb_priority_main
  *
  * @brief The main argument handler for the set PCB priority command.
  *    Accepted formats: pcb setpriority <name> <priority>
  *                      pcb setpriority --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int set_pcb_priority_main(int argc, char ** argv);

 /**
  * @name show_pcb_main
  *
  * @brief The main argument handler for the show PCB commands.
  *    Accepted formats: pcb show [name]
  *                      pcb show -all
  *                      pcb show -ready
  *                      pcb show -blocked
  *                      pcb show --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int show_pcb_main(int argc, char ** argv);

#if WITH_R2_TEMP_CMD
 /**
  * @name create_pcb_main
  *
  * @brief The main argument handler for the create PCB command.
  *    Accepted formats: pcb create <name> <type> <priority>
  *                      pcb create --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int create_pcb_main(int argc, char ** argv);

 /**
  * @name delete_pcb_main
  *
  * @brief The main argument handler for the delete PCB command.
  *    Accepted formats: pcb del <name>
  *                      pcb del --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int delete_pcb_main(int argc, char ** argv);

 /**
  * @name block_pcb_main
  *
  * @brief The main argument handler for the block PCB command.
  *    Accepted formats: pcb block <name>
  *                      pcb block --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int block_pcb_main(int argc, char ** argv);

 /**
  * @name unblock_pcb_main
  *
  * @brief The main argument handler for the unblock PCB command.
  *    Accepted formats: pcb unblock <name>
  *                      pcb unblock --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0 when finished execution.
  */
 int unblock_pcb_main(int argc, char ** argv);
#endif

#endif
