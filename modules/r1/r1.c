/**
 * @file r1.c
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief The main code file for Module R1
 * @version R1
 *
 * This file contains setdate, getdate, settime, gettime, help functions, commandhander and command line parser
 */

#include "r1.h"
#include "../mpx_supt.h"
#include "sys_clock.h"
#include <string.h>
#include <core/serial.h>
#include <core/io.h>

#define USER_INPUT_BUFFER_SIZE 1000
#define MAX_ARGC 50
#define MOD_VERSION "R1"
#define COMPLETION "02/05/2016"

static int run_mpx = 1;
static void load_functions();

/** @brief  Name: exe_function.
 *
 * Description: Executes the fucntions within the program. Accepts command from the user and refers to the command input.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */
static int exe_function(int argc, char ** argv)
{
    int i = 0;
    for(i = 0; i < NUM_OF_FUNCTIONS; i++)
    {
   	 if(!strcmp(functions[i].nameStr, argv[1]))
   	 {
   		 return functions[i].function(argc, argv);
   	 }
    }
    printf("There is no function called %s. Please refer to \"help\"\n", argv[1]);
    return 0;
}

/** @brief Name: version
 *
 *	Description: displays the version of the system currently running.
 *  @param argc - The number of tokens found.
 *  @param argv - The array of tokens.
 * 	@return 0
 */
static int version(int argc, char** argv)
{
    if (argc >= 2 && !strcmp(argv[2], "--help"))
   	 printf("%s", functions[VERSION].help);
    else
   	 printf("\nMPX-THUNDER-KRAKENS\nCOMPLETION\t%s\nVERSION \t%s\n\n", COMPLETION, MOD_VERSION);

    return 0;
}

/** @brief Name: shutdown
 *
 *	Description: Closes all functions, and shuts down the system.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * 	@return run_mpx
 */
static int shutdown(int argc, char** argv)
{
    if (argc >= 3 && !strcmp(argv[2], "--help"))
    {
   	 printf("%s", functions[VERSION].help);
   	 return run_mpx;
    }
    else if (argc >= 3)
    {
   	 printf("ERROR: Invalid arguments. Please refer to \"help\"\n");
    }

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

/** @brief Name: help_usages
 *
 *	Description: shows every help function available.
 *	@param int start_from - user input
 *  @return  1
 */
static int help_usages(int start_from)
{
    if (start_from)
   	 printf("\nAvailable mpx functions:\n");

    int i;
    for (i = start_from; i < NUM_OF_FUNCTIONS; i++)
    {
   	 printf("\tusage:\t%s\n", functions[i].usage);
    }
    printf("\n");
    return 1;
}

/** @brief Name: help_function
 *
 *	Description: displays help text for the specified function
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * 	@return 1
 */
static int help_function(int argc, char** argv)
{
    int i;
    if (argc >= 2 && !strcmp(argv[1], "--help"))
    {
   	 printf("\n%s\n", functions[HELP].help);
   	 return 1;
    }
    else if (argc == 2 && !strcmp(argv[1], "mpx"))
    {
   	 return help_usages(VERSION);
    }
    else if (argc == 2)
    {
   	 for (i = 0; i < NUM_OF_FUNCTIONS; i++)
   	 {
   		 if(!strcmp(functions[i].nameStr, argv[1]))
   		 {
   			 printf("\n%s\n", functions[i].help);
   			 return 1;
   		 }
   	 }
   	 return 0;
    }
    printf("ERROR: Invalid arguments. Please refer to \"help\"\n");
    return 0;
}

/** @brief Name: commhand
 *
 *	Description: Accepts and handles commands from the user.
 *	@param User input
 * 	@return 0
 */
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
    load_functions();

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
   			 help_usages(VERSION);
   	 }
   	 else if (!strcmp(argv[0], "help"))
   	 {
   		 if (argc > 1)
   			 functions[HELP].function(argc, argv);
   		 else
   			 help_usages(HELP);
   	 }
   	 else if (argc > 0)
   	 {
   		 printf("There is no program called \"%s\". Please refer to \"help\"\n", argv[0]);
   	 }
   	 argc = 0;  //reset the argument list
    }
    return 0;
}

/** @brief Name: CommandParserStat
 *
 *	Description: The stats of the command parser
 */
enum CommandPaserStat
{
    NotWriting,
    NormalWriting,
    DoubleQuoteWriting,
    SingleQuoteWriting
};

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


static void load_functions()
{//This is a list of functions that associate with the name of them.

    //Note: change the value in the #define NUM_OF_FUNCTIONS in r1.h & adding function's index before adding more here
    functions[HELP].nameStr = "help"; functions[HELP].function = &help_function; functions[HELP].usage = "help [function]";
    functions[HELP].help = "\nhelp : help [function]\n\n\tIf used alone, displays usage instructions for every function\
    or when used with an individual functions, gives a detailed summary of how to use that functions.\n\n\
    Arguments:\n\t\tfunction  The function to be described.\n\n\
    Exit Status:\n\t\tReturns success unless the given function is not valid function.\n\n";

    functions[VERSION].nameStr = "-version"; functions[VERSION].function = &version; functions[VERSION].usage = "mpx -version";
    functions[VERSION].help = "\nversion : -version\n\n\tPrints the current version of MPX and completion date.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[GETTIME].nameStr = "gettime"; functions[GETTIME].function = &get_time_main; functions[GETTIME].usage = "mpx gettime";
    functions[GETTIME].help = "\ngettime : gettime\n\n\tDisplays the current system's time in the format HH:MM:SS.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[SETTIME].nameStr = "settime"; functions[SETTIME].function = &set_time_main; functions[SETTIME].usage = "mpx settime [HH:MM:SS]";
    functions[SETTIME].help = "\nsettime : settime [HH:MM:SS]\n\n\tSets the system's time to the specified HH:MM:SS.\
    \n\nArguments:\n\tHH  Specified hours ranging from 00 (midnight) to 23 (1AM.)\n\tMM  Specified minutes.\n\tSS  Specified seconds.\n\n\
    Exit Status:\n\tReturns success unless given hours, minutes, and/or seconds are incorrect values.\n\n";

    functions[GETDATE].nameStr = "getdate"; functions[GETDATE].function = &get_date_main; functions[GETDATE].usage = "mpx getdate";
    functions[GETDATE].help = "\ngetdate : getdate\n\n\tDisplays the current system's date in the format MM/DD/YY.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[SETDATE].nameStr = "setdate"; functions[SETDATE].function = &set_date_main; functions[SETDATE].usage = "mpx setdate [MM/DD/YY]";
    functions[SETDATE].help = "\nsetdate : setdate [MM/DD/YY]\n\n\tSets the system's date in the specified MM/DD/YY.\n\n\
    \n\nArguments:\n\tMM  Specified month ranging from 1 to 12.\n\tDD  Specified day ranging from 1 to 31.\n\tYY  Specified year.\n\n\
    Exit Status:\n\tReturns success unless given month, day, and/or year are incorrect values.\n\n";

    functions[SHUTDOWN].nameStr = "-shutdown"; functions[SHUTDOWN].function = &shutdown; functions[SHUTDOWN].usage = "mpx -shutdown";
    functions[SHUTDOWN].help = "\nshutdown : -shutdown\n\n\tShuts down the operating system.\n\n\
    Exit Status: Always Succeeds.\n\n";

}
