#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "../errno.h"
#include "ansi.h"

#define MAX_ARGC 50
#define USER_INPUT_BUFFER_SIZE 1000

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
} __attribute__ ((packed));


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
   	 if(isspace(*CmdStr) && WritingStat == NotWriting)
   	 {
   		 //Space before writing, just ignore it.
   	 }
   	 else if( (isspace(*CmdStr) && WritingStat == NormalWriting) ||
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

int main(int argc, char **argv)
{
    static int inner_argc = 0;
    static char ActArgArray[MAX_ARGC][USER_INPUT_BUFFER_SIZE];
    char * inner_argv[MAX_ARGC];
    int i = 0;
    for(i = 0; i < 50; i++)
    {
        inner_argv[i] = ActArgArray[i];
    }
    
    if (argv[1] == NULL) 
    {
        printf("\nError: Please enter the image filepath.\n\n");
        return 0;
    }
    
    
    if(load_image_file(argv[1]) == E_FILE_NF)
    {
        printf("\nError: Could not find file %s.\n\n", argv[1]);
        return 0;
    }
    
    if (argv[2])
    {
        printf("\nTODO: THUNDERKRAKENS - Implement me!\n\n");
    }
    
    // printf("%s%sTEST%s\n", T_BOLD, T_CYAN, T_RESET);
    folder_manager_init();
    
    print_boot_sec_info(boot_sec);
    //print_curr_dir_entry_list();
    list_curr_file_and_dir();
    //ls();
    
    char * command_str = NULL;
    size_t command_str_len = 0;

    int is_run = 1;
    
    while(is_run)
    {
        print_curr_path();
        printf("> ");

        getline(&command_str, &command_str_len, stdin);
        command_line_parser(command_str, &inner_argc, inner_argv, MAX_ARGC, USER_INPUT_BUFFER_SIZE);
        int i;
        
        /*
        // Needs fixed
        char arg1[20];
        char arg2[20];
        char arg3[20];
        
        sscanf(command_str, "%s %s %s", arg1, arg2, arg3);

        
        if (!strcmp(arg1, "print"))
        {
            if (!strcmp(arg2, "boot"))
            {
                print_boot_sec_info(boot_sec);
            } 
            continue; //goes back to beginning of while loop
        }
        
        if (!strcmp(arg1, "rename")) 
        {
            rename_file(arg2, arg3);
            continue;
        }
        
        //needs implemented
        if (!strcmp(arg1, "cd"))
        {
            continue;
        }
        
        if (!strcmp(arg1, "ls"))
        {
            if (!strcmp(arg2, "-l"))
            {
                print_curr_dir_entry_list();
            } else {
                ls();
            }
            continue;
        }
        */
        
        if (inner_argc && !strcmp(inner_argv[0], "exit"))
        {
            is_run = 0;
        }
        
        if (inner_argc && !strcmp(inner_argv[0], "ls"))
        {
            if(!strcmp(inner_argv[1], "-l"))
                print_curr_dir_entry_list();
            else
                list_curr_file_and_dir();
        }
        
        if (inner_argc == 2 && !strcmp(inner_argv[0], "cd"))
        {
            if(!strcmp(inner_argv[1], "."))
            {
                
            }
            else if(!strcmp(inner_argv[1], ".."))
            {
                pop_folder();
            }
            else
                push_folder(get_entry(inner_argv[1]));
        }
        
        inner_argc = 0;
    }
    
    // free(command_str);
    // command_str = NULL;
    clean_buffers();
    return 0;
}



#if 0
------------------------------
cs450_2.img 
~ ls
1984.TXT SUBDIR   WVU.JPG

~ ls -a
.          .Trashes   .fseventsd SUBDIR
..         ._.Trashes 1984.TXT   WVU.JPG

~ cd SUBDIR
~ ls
ANDREW.TXT WOW.JPG

~ ls -a
.          ..         ANDREW.TXT WOW.JPG

-------------------------------
450_3.img
~ ls
CS111     EX14.C    EX17.C    EX7.C     FELEEX6.C WREATHS

~ ls -a
.          .Trashes   .fseventsd EX14.C     EX7.C      WREATHS
..         ._.Trashes CS111      EX17.C     FELEEX6.C

~ cd CS111
~ ls 
BON1.DOC     CRUISES.TXT  HMK3.DOC     MAZE2.TXT    READMAZE.TXT TESTFILE.DAT
BTREES.HTM   EMPLOYEE.DAT HOURLY.DAT   POLY.RTF     REV1.DOC     TMAZE1.TXT
CODE.RTF     EXAMPLE1.TXT INSTDES.TXT  PRO2SUM2.TXT SLINLST.JAV  TMAZE2.TXT
CODING.DOC   GCD.TXT      INTNOD~1.JAV PROJ5.DAT    STKSPEC.DOC
COMM.DAT     HMK1.DOC     MAZE1.TXT    QUESPEC.DOC  STUDENT.TXT

~ ls -a
.            CODING.DOC   GCD.TXT      INTNOD~1.JAV PROJ5.DAT    STKSPEC.DOC
..           COMM.DAT     HMK1.DOC     MAZE1.TXT    QUESPEC.DOC  STUDENT.TXT
BON1.DOC     CRUISES.TXT  HMK3.DOC     MAZE2.TXT    READMAZE.TXT TESTFILE.DAT
BTREES.HTM   EMPLOYEE.DAT HOURLY.DAT   POLY.RTF     REV1.DOC     TMAZE1.TXT
CODE.RTF     EXAMPLE1.TXT INSTDES.TXT  PRO2SUM2.TXT SLINLST.JAV  TMAZE2.TXT

~ cd ../WREATHS
~ ls
EMPLOYEE.JAV POSTER1.PDF  POSTER2.PDF

~ ls -a
.            ..           EMPLOYEE.JAV POSTER1.PDF  POSTER2.PDF

//things we still need:
//      print boot sec info
//      print root dir
//      change directory
//      list dir
//      Type
//      Rename
//      handle arrow keys
//Extra Credit:
//      Move
//      Adding file

#endif 