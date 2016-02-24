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
 int suspend_pcb_main(int argc, char ** argv);

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
 int resume_pcb_main(int argc, char ** argv);

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
 int set_pcb_priority_main(int argc, char ** argv);

 /**
  * @name show_pcb_main.
  *
  * @brief The main function for the "Show PCB", "Show all Processes", "Show Ready Processes", and "Show Blocked Processes".
  *    Accepted formats: pcb show [name]
  *                      pcb show -all
  *                      pcb show -ready
  *                      pcb show -blocked
  *                      pcb show --help
  * @param argc  The number of tokens found.
  * @param argv  The array of tokens.
  * @return  0
  */
 int show_pcb_main(int argc, char ** argv);

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
 int create_pcb_main(int argc, char ** argv);

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
 int delete_pcb_main(int argc, char ** argv);

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
 int block_pcb_main(int argc, char ** argv);

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
 int unblock_pcb_main(int argc, char ** argv);

#endif
