/**
 * @file r1.c
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief The commandhander and functions associations for Module R1
 * @version R1
 *
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

/**
* A structure to represent each function
*/
typedef struct
{
	char * nameStr; /** fuction's name */
	int (*function) (int argc, char ** argv); /** the function */
	char * usage; /** function's usage or use cases */
	char * help; /** function's help information */
} function_name;

/**
 * @name: fucntion_name.
 *
 * @brief The array that holds the associations among name, pointer to actual function, usage msg, and help msg.
 */
static function_name functions[NUM_OF_FUNCTIONS];

static int run_mpx = 1;
static void load_functions();

/**
 * @name: exe_function.
 * @brief Executes the specific fucntion.
 *
 * @param argc  	The number of tokens.
 * @param argv  	The array of tokens.
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

/** @name: version
 *
 *	@brief displays the version of the system currently running.
 *
 *  @param argc  	The number of tokens.
 *  @param argv  	The array of tokens.
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

/**
 * @name: shutdown
 *
 * @brief Closes all functions, and shuts down the system.
 *
 * @param argc 	The number of tokens found.
 * @param argv 	The array of tokens.
 *
 * @return 0 for shutdown, 1 for keep running.
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

/** @name help_usages
 *
 *	@brief shows usage message for each function.
 *
 *	@param start_from 	the index of the beginning function.
 *
 *  @return  0
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
    return 0;
}

/** @name: help_function
 *
 *	@brief displays help text for all functions.
 * 	@param argc 	The number of tokens.
 * 	@param argv 	The array of tokens.
 *
 * 	@return 0
 */
static int help_function(int argc, char** argv)
{
    int i;
    if (argc >= 2 && !strcmp(argv[1], "--help"))
    {
   	 printf("\n%s\n", functions[HELP].help);
   	 return 0;
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
   			 return 0;
   		 }
   	 }
   	 return 0;
    }
    printf("ERROR: Invalid arguments. Please refer to \"help\"\n");
    return 0;
}

/** @name commhand
 *
 *	@brief Accepts and handles commands from the user.
 *
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

/** @name CommandParserStat
 *
 *	@brief The status of the command parser
 */
enum CommandPaserStat
{
    NotWriting,
    NormalWriting,
    DoubleQuoteWriting,
    SingleQuoteWriting
};


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

/**
	* @name 	load_functions
	* @brief 	load the function associations among name, pointer to actual function, usage msg, and help msg.
	*
	* @return void
	*/
static void load_functions()
{//This is a list of functions that associate with the name of them.

    //Note: change the value in the #define NUM_OF_FUNCTIONS in r1.h & adding function's index before adding more here
    functions[HELP].nameStr = "help"; functions[HELP].function = &help_function; functions[HELP].usage = "help [function]";
    functions[HELP].help = "\nhelp : help [function]\n\n\tIf used alone, displays usage instructions for every function\
    or when used with an individual functions, gives a detailed summary of how to use that functions.\n\n\
    Arguments:\n\t\tfunction  The function to be described.\n\n\
    Exit Status:\n\t\tReturns success unless the given function is not valid function.\n\n";

    functions[VERSION].nameStr = "-version"; functions[VERSION].function = &version; functions[VERSION].usage = "mpx -version";
    functions[VERSION].help = "\nversion : mpx -version\n\n\tPrints the current version of MPX and completion date.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[GETTIME].nameStr = "gettime"; functions[GETTIME].function = &get_time_main; functions[GETTIME].usage = "mpx gettime";
    functions[GETTIME].help = "\ngettime : mpx gettime\n\n\tDisplays the current system's time in the format HH:MM:SS.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[SETTIME].nameStr = "settime"; functions[SETTIME].function = &set_time_main; functions[SETTIME].usage = "mpx settime [HH:MM:SS]";
    functions[SETTIME].help = "\nsettime : mpx settime [HH:MM:SS]\n\n\tSets the system's time to the specified HH:MM:SS.\
    \n\nArguments:\n\tHH  Specified hours ranging from 00 (midnight) to 23 (1AM.)\n\tMM  Specified minutes.\n\tSS  Specified seconds.\n\n\
    Exit Status:\n\tReturns success unless given hours, minutes, and/or seconds are incorrect values.\n\n";

    functions[GETDATE].nameStr = "getdate"; functions[GETDATE].function = &get_date_main; functions[GETDATE].usage = "mpx getdate";
    functions[GETDATE].help = "\ngetdate : mpx getdate\n\n\tDisplays the current system's date in the format MM/DD/YY.\n\n\
    Exit Status: Always Succeeds.\n\n";

    functions[SETDATE].nameStr = "setdate"; functions[SETDATE].function = &set_date_main; functions[SETDATE].usage = "mpx setdate [MM/DD/YY]";
    functions[SETDATE].help = "\nsetdate : mpx setdate [MM/DD/YY]\n\n\tSets the system's date in the specified MM/DD/YY.\n\n\
    \n\nArguments:\n\tMM  Specified month ranging from 1 to 12.\n\tDD  Specified day ranging from 1 to 31.\n\tYY  Specified year.\n\n\
    Exit Status:\n\tReturns success unless given month, day, and/or year are incorrect values.\n\n";

    functions[SHUTDOWN].nameStr = "-shutdown"; functions[SHUTDOWN].function = &shutdown; functions[SHUTDOWN].usage = "mpx -shutdown";
    functions[SHUTDOWN].help = "\nshutdown : mpx -shutdown\n\n\tShuts down the operating system.\n\n\
    Exit Status: Always Succeeds.\n\n";

}

/**
 *@name   print_help
 *@brief  prints the help message of a certain function that specified by the index number
 *
 *@param  function_index  The index number of that function.
 *
 *@return void
 */
void print_help(const int function_index)
{
  printf("%s", functions[function_index].help);
}
