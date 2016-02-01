#include "r1.h"
#include "../mpx_supt.h"
// #include "sys_time.h"
#include <string.h>
#include <core/serial.h>

#define USER_INPUT_BUFFER_SIZE 1000

int commhand()
{
	static char userInput[USER_INPUT_BUFFER_SIZE];
	
	printf("In R1 commhand...\n");
	printf("input > ");
	GetInputln(userInput, 100, WithEcho);
	printf("Your inputs: %s\n", userInput);
	
	return 0;
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
