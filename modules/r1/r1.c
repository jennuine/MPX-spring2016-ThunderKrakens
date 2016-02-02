#include "r1.h"
#include "../mpx_supt.h"
#include "sys_clock.h"
#include <string.h>
#include <core/serial.h>
#include <core/io.h>

#define USER_INPUT_BUFFER_SIZE 1000
#define MAX_ARGC 50
#define NUM_OF_FUNCTIONS 4
#define VERSION "R1"
#define COMPLETION "02/05/2016"

typedef struct
{
	char * nameStr;
	int (*function) (int argc, char ** argv);
	char * usage;
	char * help;
} function_name;

static int run_mpx = 1;
static function_name functionList[NUM_OF_FUNCTIONS];

static int version();
static int shutdown();

static int exe_function(int argc, char ** argv)
{//This is a list of functions that associate with the name of them.
	functionList[0].nameStr = "gettime"; functionList[0].function = &get_time_main; functionList[0].usage = "mpx gettime";
	functionList[0].help = "gettime : gettime\n\n\tDisplays the current system's time in the format HH:MM:SS.\n\n\
		Exit Status: Always Succeeds.\n";
	functionList[1].nameStr = "settime"; functionList[1].function = &set_time_main; functionList[1].usage = "mpx settime [HH:MM:SS]";
	functionList[1].help = "settime : settime [HH:MM:SS]\n\n\tSets the system's time to the specified HH:MM:SS.\
		\n\nArguments:\n\tHH\tSpecified hours ranging from 00 (midnight) to 23 (1AM.)\n\tMM\tSpecified minutes.\n\tSS\tSpecified seconds.\n\
		Exit Status: Always Succeeds.\n";
	functionList[2].nameStr = "-version"; functionList[2].function = &version; functionList[2].usage = "mpx -version";
	functionList[2].help = "version : -version\n\n\tPrints the current version of MPX and completion date.\n\nExit Status: Always Succeeds.\n";
	functionList[3].nameStr = "-shutdown"; functionList[3].function = &shutdown; functionList[3].usage = "mpx -shutdown";
	functionList[3].help = "shutdown : -shutdown\n\n\tShuts down the operating system.\n\nExit Status: Always Succeeds.\n";

	//Note: change the value in the #define NUM_OF_FUNCTIONS before adding more at here!

	int i = 0;
	for(i = 0; i < NUM_OF_FUNCTIONS; i++)
	{
		if(!strcmp(functionList[i].nameStr, argv[1]))
		{
			return functionList[i].function(argc, argv);
		}
	}

	printf("There is no function called %s. Please refer to \"help\"\n", argv[1]);
	return 0;
}

static int version ()
{
		printf("\nMPX-THUNDER-KRAKENS\nCOMPLETION\t%s\nVERSION\t%s\n\n", COMPLETION, VERSION);
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

static void list_functions()
{
	printf("\nAvailable mpx functions:\n");

	int i;
	for (i = 0; i < NUM_OF_FUNCTIONS; i++)
	{
		printf("\tusage: %s\n", functionList[i].usage);
	}
	printf("\n");
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
		if(!strcmp(argv[0], "mpx"))
		{
			if (argc > 1)
				exe_function(argc, argv);
			else
				list_functions();
		}
		else
		{
			printf("There is no program called \"%s\".\n", argv[0]);
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
