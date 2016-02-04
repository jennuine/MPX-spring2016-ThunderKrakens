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

function_name functions[NUM_OF_FUNCTIONS];

int commhand();

void command_line_parser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen);

#endif
