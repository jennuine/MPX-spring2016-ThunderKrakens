/**
 * @file r1.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief The commandhander and functions associations for Module R1
 * @version R1
 *
 */

#ifndef _R1_H
#define _R1_H

enum comm_type 
{
    mpx,
    pcb,
    help
} __attribute__ ((packed));


#define HELP        0

#define POS_OF_MPX 1

#define VERSION     1
#define GETTIME     2
#define SETTIME     3
#define GETDATE     4
#define SETDATE     5
#define SHUTDOWN    6
#define YIELD       7
#define LOADR3      8

#define NUM_MPX_FUNCTIONS 9

#define POS_OF_PCB 9

#define SUSPDPCB    9
#define RESUMEPCB   10
#define SETPCBPRIO  11
#define SHOWPCB     12
//#define CREATEPCB   13
//#define DELPCB      14
//#define BLOCKPCB    15
//#define UNBLKPCB    16

#define NUM_OF_FUNCTIONS 13

/** @name commhand
 *
 *	@brief Accepts and handles commands from the user.
 *
 * 	@return VOID
 */
void commhand();

/**
* @name   command_line_parser
* @brief  Splits the complete command line into tokens by space, single quote, or double quote.
*
* @param  CmdStr     The complete input command.
* @param  argc       The number of tokens found.
* @param  argv       The array of tokens.
* @param  MaxArgNum  The maximum number of tokens that array can hold.
* @param  MaxStrLen  The maximum length of each token that string can hold.
*
* @return void
*/
void command_line_parser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen);

/**
 *@name   print_help
 *@brief  prints the help message of a certain function that specified by the index number
 *
 *@param  function_index  The index number of that function.
 *
 *@return void
 */
void print_help(const int function_index);

int help_usages(enum comm_type type);

#endif
