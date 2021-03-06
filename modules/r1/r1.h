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

/**
 * @brief Command types
 */
enum comm_type 
{
    mpx, /**< MPX System command */
    pcb, /**< Process Control Block command */
    mcb, /**< Memory Control Block command */
    help /**< Help command */
} __attribute__ ((packed));

#include "../cmd_orders.h"


/** 
 * @name commhand
 *
 *	@brief Accepts and handles commands from the user.
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
*/
void command_line_parser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen);

/**
 *@name   print_help
 *@brief  Prints the help message of a certain function that specified by the index number
 *
 *@param  function_index  The index number of that function.
 *
 */
void print_help(const int function_index);

/**
 * @name help_usages
 * @brief Displays all the usage case of the specified command type.
 * 
 * @param   comm_type   The command type.
 * @return  When finished execution returns 0.
 */
int help_usages(enum comm_type type);

#endif
