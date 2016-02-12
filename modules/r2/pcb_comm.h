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

 int suspend_pcb_main(int argc, char ** argv);

 int resume_pcb_main(int argc, char ** argv);

 int set_pcb_priority_main(int argc, char ** argv);

 int show_pcb_main(int argc, char ** argv);

 int show_all_processes_main(int argc, char ** argv);

 int show_ready_processes_main(int argc, char ** argv);

 int show_blocked_processes_main(int argc, char ** argv);

 int create_pcb_main(int argc, char ** argv);

 int delete_pcb_main(int argc, char ** argv);

 int block_pcb_main(int argc, char ** argv);

 int unblock_pcb_main(int argc, char ** argv);

#endif
