#include "r1.h"
#include "../mpx_supt.h"
#include "sys_clock.h"
#include <string.h>
#include <core/serial.h>

#include <core/io.h>

#define USER_INPUT_BUFFER_SIZE 1000

static int shutdown(const char* cmd_str)
{
	if (strcmp(cmd_str, "mpx-shutdown") != 0)
		return 1;
	else
	{
		char ans[] = { 0, 0 };
		printf("\nAre you sure you would like to shutdown?\n( Type y/n )\n");

		while (!ans[0])
		 	if (inb(COM1 + 5) & 1)
				ans[0] = inb(COM1);

		if (strcmp(ans, "y") == 0 || strcmp(ans, "Y") == 0)
			return 0;
		else if (strcmp(ans, "n") == 0 || strcmp(ans, "N") == 0)
			return 1;
		else
			{
				printf("Error: %s was invalid.\n\n", ans);
				return 1;
			}
	}
}

int commhand()
{
	static char userInput[USER_INPUT_BUFFER_SIZE];

	printf("In R1 commhand...\n");
	printf("input > ");
	GetInputln(userInput, 100, WithEcho);
	printf("Your inputs: %s\n", userInput);

	return shutdown(userInput);
}

enum CommandPaserStat
{
	NotWriting,
	NormalWriting,
	DoubleQuoteWriting,
	SingleQuoteWriting
};

void CommandLineParser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen)
{
	enum CommandPaserStat WritingStat = NotWriting;
	char * LinePtr = NULL;
	while(*CmdStr && *argc <= MaxArgNum - 1)
	{
		if(*CmdStr == ' ' && WritingStat == NotWriting)
		{
			//Space before writing, just ignore it.
		}
		else if( (*CmdStr == ' ' && WritingStat == NormalWriting) ||
			 (*CmdStr == '\"' && WritingStat == DoubleQuoteWriting) ||
			 (*CmdStr == '\'' && WritingStat == SingleQuoteWriting) ||
			 (LinePtr && LinePtr - argv[*argc] >= MaxStrLen - 1) )
		{//End of writing
			*LinePtr++ = '\0';
			(*argc)++;
			WritingStat = NotWriting;
		}
		else if(*CmdStr == '\"' && WritingStat == NotWriting)
		{//Begin Double Quote Writing
			WritingStat = DoubleQuoteWriting;
			LinePtr = argv[*argc];
			CmdStr++;
		}
		else if(*CmdStr == '\'' && WritingStat == NotWriting)
		{//Begin Single Quote Writing
			WritingStat = SingleQuoteWriting;
			LinePtr = argv[*argc];
			CmdStr++;
		}else if(WritingStat == NotWriting)
		{//Begin Normal Writing
			WritingStat = NormalWriting;
			LinePtr = argv[*argc];
		}

		if(*CmdStr && WritingStat > NotWriting)
		{
			*LinePtr++ = *CmdStr;
		}
		CmdStr++;
	}

	if(WritingStat > NotWriting)
	{
		*LinePtr++ = '\0';
		(*argc)++;
		WritingStat = NotWriting;
	}
}
