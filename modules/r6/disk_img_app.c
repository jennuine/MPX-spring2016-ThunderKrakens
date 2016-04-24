#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "disk_file_manager.h"
#include "../errno.h"
#include <stdio.h>
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

int main(int argc, char ** argv)
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
    
    folder_manager_init();
    
    print_boot_sec_info(boot_sec);
    //list_dir_entry_report();
    //list_dir_entry_short();
    printf("%s%sFree space: %d Bytes.%s%s\n\n", T_BOLD, T_RED, calc_free_space(), T_NRM, T_BOLD_OFF);
    
	char command_str[USER_INPUT_BUFFER_SIZE];

    int is_run = 1;
    
    while(is_run)
    {
        print_curr_path();
        printf("> ");
        
		fgets(command_str, USER_INPUT_BUFFER_SIZE, stdin);
        command_line_parser(command_str, &inner_argc, inner_argv, MAX_ARGC, USER_INPUT_BUFFER_SIZE);
        
        if (inner_argc && !strcmp(inner_argv[0], "exit"))
        {
            is_run = 0;
        }
        else if (inner_argc && !strcmp(inner_argv[0], "ls"))//list
        {
            list_main(inner_argc, inner_argv);
        }
        else if (!strcmp(inner_argv[0], "cd"))//change directory
        {
            change_dir_main(inner_argc, inner_argv);
        }
        else if (inner_argc && !strcmp(inner_argv[0], "ps"))//Print Boot info
        {
            print_boot_sec_main(inner_argc, inner_argv);
        }
        else if (inner_argc == 3 && (!strcmp(inner_argv[0], "rn") || !strcmp(inner_argv[0], "rename")))//rename
        {
            rename_main(inner_argc, inner_argv);
        }
        else if (inner_argc == 2 && (!strcmp(inner_argv[0], "less") || !strcmp(inner_argv[0], "type"))) //type 
        {
            struct dir_entry_info * file_entry = get_entry(inner_argv[1]);
            type_file(file_entry);
        }
        else if (!strcmp(inner_argv[0], "mv"))//move
        {
            move_main(inner_argc, inner_argv);
        } else {
            printf("\n%s%sERROR:%s No such function.\n\n", T_BOLD, T_RED, T_RESET);
        }
        
        inner_argc = 0;
    }
    
    clean_buffers();
    return 0;
}


//NOTE: When your code is ready to test, you have to move it out from the "#if 0 ... #endif" block.
//====== main functions ================



int print_boot_sec_main(int argc, char ** argv) {

    if (argc = 2 && !strcmp(argv[1], "--help"))
    {
        //print_help();
    }
    else if(argc == 1)
    {
        print_boot_sec_info(boot_sec);
    }
    else
    {
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"Print boot sec --help\"\n\n", T_BOLD, T_RED, T_RESET);
        return 0;
    }
    
    return 0;
}


int change_dir_main(int argc, char ** argv){
    if (argc == 2 && !strcmp(argv[1], "--help")){
        //printf()
    }
    else if(argc == 2){
        change_dir(argv[1]);
    }
    else{
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"cd --help\"\n\n", T_BOLD, T_RED, T_RESET);
    }
     return 0;
}

int rename_main(int argc, char ** argv) {
    
    if (argc == 3 && !strcmp(argv[1], "--help")) 
    {
        printf("rename : rn [old filename] [new filename]\n\
        \n\tRenames the file.\n\n\
        \n\nArguments:\n\tprocessName  String process name\n\
        Exit Status:\n\tChanges filename if the length is between [1, 8].\n\n");
    } else if (argc == 3) {
        char * file_path = malloc(strlen(argv[1]));
        memcpy(file_path, argv[1], strlen(argv[1]));
        int len = strlen(file_path);
        int temp = len - 1;
        for(; temp >= 0 && file_path[temp] != '/' && file_path[temp] != '\\'; temp--);
        temp++;
        if(temp == 0 || temp == 1)
            memcpy(file_path, ".\0", 2);
        else
            file_path[temp - 1] = '\0';
        //printf("*DEBUG parent path: %s\n", file_path);
        error_t errno = rename_entry(get_entry(file_path), get_entry(argv[1]), argv[2]);
        free(file_path);
        switch(errno)
        {
            case E_NOERROR:
                printf("Renamed successfully!\n");
                break;
            case E_NULL_PTR:
                printf("\n%s%sERROR:%s Could not find file \"%s\"\n\n", T_BOLD, T_RED, T_RESET, argv[1]);
                break;
            case E_INVPARA:
                printf("\n%s%sERROR:%s There is already a file named \"%s\" in the destination!\n\n", T_BOLD, T_RED, T_RESET, argv[2]);
                break;
            case E_INVSTRF:
                printf("\t%s The length of the filename must within the range of [1, 8]. %s\n\n", T_ITCS, T_ITCS_OFF);
                break;
        }
    } else {
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"rn --help\"\n\n", T_BOLD, T_RED, T_RESET);
    }
    return 0;
}

int list_main(int argc, char ** argv) {
    
    if(argc == 2 && !strcmp(argv[1], "-l"))
        list_dir_entry_report();
    else if (argc == 1)
        list_dir_entry_short();
    else
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"ls --help\"\n\n", T_BOLD, T_RED, T_RESET);
    
    return 0;
}

int move_main(int argc, char ** argv)
{
    if(argc == 4 && !strcmp(argv[1], "-o"))
    {
        struct dir_entry_info * file_entry = get_entry(argv[2]);
        error_t errno = extract_file(file_entry, argv[3]);
        switch(errno)
        {
            case E_NOERROR:
                printf("The file had been successfully extracted!\n");
                break;
            default:
                printf("ERROR: Could not extract the file!\n");
                switch(errno)
                {
                    case E_NULL_PTR:
                        printf("\t Could not found the source within the image!\n");
                        break;
                    case E_INVPARA:
                        printf("\t The source you had indicated is not a file, or the destination cannot be written!\n");
                        break;
                }
                break;
        }
    }
    else if(argc == 4 && !strcmp(argv[1], "-m"))
    {
        struct dir_entry_info * file_entry = get_entry(argv[2]);
        struct dir_entry_info * dest_entry = get_entry(argv[3]);
        error_t errno = move_file(file_entry, dest_entry);
        switch(errno)
        {
            case E_NOERROR:
                printf("The file had been successfully moved!\n");
                break;
            default:
                printf("ERROR: Could not move the file!\n");
                switch(errno)
                {
                    case E_NULL_PTR:
                        printf("\t Could not found the source or destination within the image!\n");
                        break;
                    case E_INVPARA:
                        printf("\t The source is not a file, or the destination is not a folder, or there is a file with the same name in the destination!\n");
                        break;
                }
                break;
        }
    }
    else if(argc == 4 && !strcmp(argv[1], "-i"))
    {
        struct dir_entry_info * dest_entry = get_entry(argv[2]);
        error_t errno = import_file(argv[3], dest_entry);
        switch(errno)
        {
            case E_NOERROR:
                printf("The file had been successfully imported!\n");
                break;
            default:
                printf("ERROR: Could not import the file!\n");
                switch(errno)
                {
                    case E_NULL_PTR:
                        printf("\t Could not found the destination within the image!\n");
                        break;
                    case E_INVPARA:
                        printf("\t The source cannot be opened, or the destination is not a folder, or there is a file with the same name in the destination!\n");
                        break;
                    case E_INVSTRF:
                        printf("\t The file name or extension of the source is too long!\n");
                        break;
                }
                break;
        }
    }
    else
    {
        printf("mv: Cannot understand the argument you had given.\n");
    }
    return 0;
}

#if 0


int print_root_dir_main(int argc, char ** argv) {
     if (argc = 2 && !strcmp(argv[1], "--help"))
    {
        //print_help(); 
    }
    else if(argc == 1)
    {
        /* print_root_dir:
        current_folder = malloc(sizeof(struct folder));      
      
        struct dir_itr * dir_entry_itr1 = init_dir_itr(current_folder->ROOT_DIR_SEC_INDEX);
        ditr_set_filter(dir_entry_itr1, 0);
    
        for(ditr_begin(dir_entry_itr1); !ditr_end(dir_entry_itr1); ditr_next(dir_entry_itr1))
        {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr1);
        print_dir_entry_info(current_entry);
        }
        
        
        */
    }
    else
    {
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"Print root sec --help\"\n\n", T_BOLD, T_RED, T_RESET);
        return 0;
    }
    
    return 0;
    
}





int type_main(int argc, char ** argv)



#endif


//======================================








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
