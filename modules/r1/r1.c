#include "r1.h"
#include "../mpx_supt.h"
#include "sys_clock.h"
#include <string.h>
#include <core/serial.h>
#include <core/io.h>

#define USER_INPUT_BUFFER_SIZE 1000
#define MAX_ARGC 50
#define NUM_OF_FUNCTIONS 3
#define NUM_OF_MPX_FUNCTIONS 2
#define VERSION "R1"

static int run_mpx = 1;

typedef struct
{
	char * nameStr;
	int (*function) (int argc, char ** argv);
} function_name;

static int version();
static int shutdown();
static int mpx_function();

static int exe_function(int argc, char ** argv)
{//This is a list of functions that associate with the name of them.
	static function_name functionList[NUM_OF_MPX_FUNCTIONS];
	functionList[0].nameStr = "gettime"; functionList[0].function = &get_time_main;
	functionList[1].nameStr = "settime"; functionList[1].function = &set_time_main;
	functionList[2].nameStr = "mpx"; functionList[2].function = &mpx_function;
	//Note: change the value in the #define NUM_OF_FUNCTIONS before adding more at here!

	int i = 0;
	for(i = 0; i < NUM_OF_FUNCTIONS; i++)
	{
		if(!strcmp(functionList[i].nameStr, argv[0]))
		{
			return functionList[i].function(argc, argv);
		}
	}

	printf("There is no function called %s. Please refer to \"help\"\n", argv[0]);
	return 0;
}

static int mpx_function(int argc, char ** argv)
{
	static function_name mpx_functions[NUM_OF_FUNCTIONS];
	mpx_functions[0].nameStr = "-version";	mpx_functions[0].function = &version;
	mpx_functions[1].nameStr = "-shutdown"; mpx_functions[1].function = &shutdown;

	int i = 0;
	for (i = 0; i < NUM_OF_MPX_FUNCTIONS; i++)
	{
		if (!strcmp(mpx_functions[i].nameStr, argv[1]))
			return mpx_functions[i].function(argc, argv);
	}

	printf("There is no mpx function called %s. Please refer to \"help\"\n", argv[1]);
	return 0;
}

static int version ()
{
		printf("\nMPX-THUNDER-KRAKENS\nVERSION\t%s\n\n", VERSION);
		return 0;
}

static int shutdown()
{

	char ans[] = { 0, 0 };
	printf("\nAre you sure you would like to shutdown?\n( Type y/n )\n");

	while (!ans[0])
	 	if (inb(COM1 + 5) & 1)
			ans[0] = inb(COM1);

	printf("\n");
	if (!strcmp(ans, "y") || !strcmp(ans, "Y"))
		return run_mpx = !run_mpx;
	else if (!strcmp(ans, "n") || !strcmp(ans, "N"))
		return run_mpx;
	else
		{
			printf("Error: %s was invalid.\n\n", ans);
			return run_mpx;
		}
}

int commhand()
{
	static char userInput[USER_INPUT_BUFFER_SIZE];
	static int argc = 0;
	static char ActArgArray[MAX_ARGC][USER_INPUT_BUFFER_SIZE];
	static char * argv[MAX_ARGC];
	int i = 0;
	for(i = 0; i < 50; i++)
	{
		argv[i] = ActArgArray[i];
	}

	printf("\nWelcome! This is Thunder Krakens Operating System!\n\n");

	while(run_mpx)
	{
		printf("Cmd > ");
		get_input_line(userInput, USER_INPUT_BUFFER_SIZE, WithEcho);
		command_line_parser(userInput, &argc, argv, MAX_ARGC, USER_INPUT_BUFFER_SIZE);
		if(argc > 0)  //if there are some argument
		{
			exe_function(argc, argv);
		}
		argc = 0;  //reset the argument list
	}
	return 0;
}

enum CommandPaserStat
{
	NotWriting,
	NormalWriting,
	DoubleQuoteWriting,
	SingleQuoteWriting
};

void command_line_parser(const char * CmdStr, int * argc, char ** argv, const int MaxArgNum, const int MaxStrLen)
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
