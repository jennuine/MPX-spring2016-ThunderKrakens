/**
 * @file r1.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief The main header for Module R1
 *
 * This file contains setdate, getdate, settime, gettime, help functions, commandhander and command line parser
 */
 
#ifndef _R1_H
#define _R1_H

typedef struct
{
	char * nameStr;
	int (*function) (int argc, char ** argv);
	char * usage;
	char * help;
} function_name;

#define HELP 0
#define VERSION 1
#define GETTIME 2
#define SETTIME 3
#define GETDATE 4
#define SETDATE 5
#define SHUTDOWN 6

#define NUM_OF_FUNCTIONS 7

/** @brief  Name: fucntion_name.
 *
 * Description: Initializes number of fucntions
 * @param NUM_OF_FUNCTION - Predefined with 6 fuctions.
 */

function_name functions[NUM_OF_FUNCTIONS];

/** @brief Name: commhand
 *
 *	Description: Accepts and handles commands from the user.
 *	@param User input
 * 	@return 0
 */

int commhand();

/**
* @brief Name: command_line_parser
*
* Description: Splits the complete command line into tokens by space, single quote, or double quote.
*
* @param CmdStr - The complete input command.
* @param argc - The number of tokens found.
* @param argv - The array of tokens.
* @param MaxArgNum - The maximum number of tokens that array can hold.
* @param MaxStrLen - The maximum length of each token that string can hold.
*
* @return void
*/

void command_line_parser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen);

#endif
