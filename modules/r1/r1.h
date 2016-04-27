/**
 * @file r1.h
 * @author Thunder Krakens
 * @date March 17, 2016
 * @brief The command handler and functions associations for Module R1
 * @version R3 & R4
 *
 */

#ifndef _R1_H
#define _R1_H

enum comm_type 
{
    mpx,
    pcb,
    mcb,
    help
} __attribute__ ((packed));

#include "../cmd_orders.h"


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
