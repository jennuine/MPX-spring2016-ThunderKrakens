#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "disk_file_manager.h"
#include "file_dir_iterator.h"
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

//Main functions' prototypes:
int print_root_dir_main(int argc, char ** argv);
int move_main(int argc, char ** argv);
int type_main(int argc, char ** argv);
int list_main(int argc, char ** argv);
int rename_main(int argc, char ** argv);
int change_dir_main(int argc, char ** argv);
int print_boot_sec_main(int argc, char ** argv);

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

int dump(char *argv)
{
    struct dir_entry_info * file_entry = get_entry(argv);
    char ext[3];
    
    if (file_entry == NULL)
    {
        printf("\n%s%sERROR:%s Could not locate file\n\n", T_BOLD, T_RED, T_RESET);
        return 0;
    } 
    
    ch_arr_to_str(ext, file_entry->extension, 3);
    if (!strcmp(ext, "TXT") || !strcmp(ext, "BAT") || !strcmp(ext, "C"))
        type_file(file_entry);
    else
    {
        char full_name[11];
        ch_arr_to_str(full_name, file_entry->file_name, 8);
        strcat(full_name, ".");
        strcat(full_name, ext);
        
        // printf("DEBUG: %s\n", full_name);
        // printf("%s\n", ext);
        
        error_t errno = extract_file(file_entry, full_name);
        printf("\n");
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
        printf("\n");
    }
    return 0;
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
    
    folder_manager_init();
    
    if (argv[2])
    {
        dump(argv[2]);
        return 0;
    }
    
    FILE * file = fopen("heading.txt", "r");
    char line[100];

    while (fgets(line, 100, file))
        printf("%s", line);
    
    fclose(file);
    printf("\n");
    
    print_boot_sec_info(boot_sec);
    //list_dir_entry_report();
    //list_dir_entry_short();
    //printf("%s%sFree space: %d Bytes.%s%s\n\n", T_BOLD, T_RED, calc_free_space(), T_NRM, T_BOLD_OFF);
    
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
        else if (inner_argc && !strcmp(inner_argv[0], "cd"))//change directory
        {
            change_dir_main(inner_argc, inner_argv);
        }
        else if (inner_argc == 1 && !strcmp(inner_argv[0], "pb"))//Print Boot info
        {
            print_boot_sec_main(inner_argc, inner_argv);
        }
        else if (inner_argc && (!strcmp(inner_argv[0], "rn") || !strcmp(inner_argv[0], "rename")))//rename
        {
            rename_main(inner_argc, inner_argv);
        }
        else if (inner_argc && !strcmp(inner_argv[0], "less")) //type 
        {
            type_main(inner_argc, inner_argv);
        }
        else if (inner_argc && !strcmp(inner_argv[0], "mv"))//move
        {
            move_main(inner_argc, inner_argv);
        }
        else if (inner_argc && !strcmp(inner_argv[0], "help"))
        {
            printf("\nAvailable commands are:\n\n\
            cd [directory] - change directory\n\
            exit - exits the program\n\
            less [file] - aka \'type\', prints contents of file\n\
            ls [option] [file] - list contents\n\
            mv [option] [path1] [path2] - moves file to indicated location\n\
            pb - print boot information\n\
            rn [old filename] [new filename] - rename file/directory\n\
            write - saves all image file changes\n\n");
        }
        else if (inner_argc == 1 && !strcmp(inner_argv[0], "write"))
        {
            write_image_file_main();
        }
        else if (inner_argc && !strcmp(inner_argv[0], "lsr"))
        {
            print_root_dir_main(inner_argc, inner_argv);
        }
        else 
        {
            printf("\n%s%sERROR:%s No such function, please run to \'help\' to list available commands.\n\n", T_BOLD, T_RED, T_RESET);
        }
        
        inner_argc = 0;
    }
    
    while(write_image_file_main());
    clean_buffers();
    return 0;
}

int write_image_file_main()
{
    printf("\n%sDo you want to save your content changes to the image?%s (Enter Y/N) \n", T_BOLD, T_BOLD_OFF);
    char user_reply[10];
    fgets(user_reply, 10, stdin);
    fflush(stdin);
    
    int len = strlen(user_reply);
    int temp = len - 1;
    for(; temp >= 0 && isspace(user_reply[temp]); temp--);
    temp++;
    user_reply[temp] = '\0';
    
    printf("\n");
    
    if(!strcmp(user_reply, "Y") || !strcmp(user_reply, "y"))
    {
        error_t errno = write_image_file();
        switch(errno)
        {
            case E_NOERROR:
                printf("Your changes have been saved successfully!\n");
                break;
            case E_FILE_NF:
                printf("Could not open the image file!\n");
                printf("Changes have NOT been saved.\n");
                break;
            default:
                printf("Some error happened when saving.\n");
                break;
        }
    }
    else if(!strcmp(user_reply, "N") || !strcmp(user_reply, "n"))
    {
        printf("Changes have NOT been saved.\n");
    }
    else
    {
        printf("%s%sERROR:%s The answer, \"%s\", you entered is invalid.\n", T_BOLD, T_RED, T_RESET, user_reply);
        return 1;
    }
    printf("\n");
    return 0;
}

//NOTE: When your code is ready to test, you have to move it out from the "#if 0 ... #endif" block.
//====== main functions ================



int print_boot_sec_main(int argc, char ** argv) {
    
    if(argc == 1)
    {
        print_boot_sec_info(boot_sec);
    }
    else
    {
        printf("\n%s%sERROR:%s Incorrect input. \n\n", T_BOLD, T_RED, T_RESET);
        return 0;
    }
    
    return 0;
}


int change_dir_main(int argc, char ** argv)
{
    if (argc == 2 && !strcmp(argv[1], "--help"))
    {
        printf("cd [directory] \t - to change directory. [directory] can be a path to a directory\n\n");
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
            case E_NAMEINV:
                printf("\t%s File name cannot contain {/\\<>\"?:|*} %s\n\n", T_ITCS, T_ITCS_OFF);
                break;
        }
    } else {
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"rn --help\"\n\n", T_BOLD, T_RED, T_RESET);
    }
    return 0;
}

void list_wildcard(char *argv, int report)
{
    char * token = strtok(argv, ".");
    
    if (!strcmp(token, "*"))
    {
        token = strtok(NULL, ".");
        char ext[3];
        strcpy(ext, token);
        
        if (!report)
            list_files_entry_ext(ext);
        else
            list_file_report("*", ext, 1);
    } else {
        
        char file_name[9];
        strcpy(file_name, token);
        token = strtok(NULL, ".");
        
        if (token != NULL && !strcmp(token, "*"))
        {
            if (!report)
                list_files_entry_name(file_name);
            else
                list_file_report(file_name, "*", 1);
                
        } else if ( token != NULL ) {
            
            char ext[3];
            strcpy(ext, token);
            list_file_report(file_name, ext, 0);
            
        } else // it is most likely a directory
            list_file_report(file_name, "", 0);
    }
}


int list_main(int argc, char ** argv) 
{
    printf("\n");
    if (argc == 2 && !strcmp(argv[1], "--help"))
    {
        printf("list : ls [option] [file]\n\n\
        Lists the contents contained in the current directory.\n\
        If a file is given then it will list the file's full details (file report)\n\
        If no file is given then lists all the contents in the directory.\n\n\
        Available [option]:  -l    list the file(s) report of the contents.\n\n\
        Optional [file]:   The file(s)/extension(s) to be listed.\n\
        \t\tThe filename may be a wildcard OR the extention may be a wildcard.\n\n\
        Example: %sls -l *.TXT%s will list all the file reports with the .TXT extension.\n\n\
        Exit Status:\tLists the contents of the directory unless a specified file is not found.\n\n", T_ITCS, T_ITCS_OFF);
    } 
    else if(argc == 2 && !strcmp(argv[1], "-l"))
        list_dir_entry_report();
        
    else if (argc == 3 && !strcmp(argv[1], "-l"))
        list_wildcard(argv[2], 1);
        
    else if (argc == 1)
        list_dir_entry_short();
    
    else if (argc == 2)
       list_wildcard(argv[1], 0);
    
    else
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"ls --help\"\n\n", T_BOLD, T_RED, T_RESET);
    
    return 0;
}


int type_main(int argc, char ** argv) {
    
    if (argc == 2 && !strcmp(argv[1], "--help"))
    {
        printf("type : less [file]\n\n\
        Prints the contents of the file to the screen, approximately a screen full at a time.\n\
        In pagination, %sq%s to quit printing contents and any other key to continue.\n\n\
        Exit Status:\tPrints file's contents unless it is not found.\n\n", T_ITCS, T_ITCS_OFF);
    
    } else if (argc == 2 && argv[1]) {
    
        struct dir_entry_info * file_entry = get_entry(argv[1]);
        if (file_entry == NULL)
        {
            printf("\n%s%sERROR:%s Could not find file \"%s\"\n\n", T_BOLD, T_RED, T_RESET, argv[1]);
            return 0;
        } 
        error_t errno = type_file(file_entry);
        
        if(errno == E_INVPARA)
        {
            printf("\n%s%sERROR:%s  Unable to print contents of file that is not \"TXT\", \"BAT\", \"C\", and \"HTM\".\n\n", T_BOLD, T_RED, T_RESET);
            return 0;
        }
        else if (errno == E_INVATTRS)  
            printf("\n%s%sERROR:%s  Unable to print contents of this file type/attribute.\n\n", T_BOLD, T_RED, T_RESET);
        
        return 0;

        
    } else
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"less --help\"\n\n", T_BOLD, T_RED, T_RESET);
    
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
                    case E_NOSPACE:
                        printf("\t This disk does not have enough space to store this file!\n");
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


int print_root_dir_main(int argc, char ** argv) {
    if (argc == 2 && !strcmp(argv[1], "--help"))
    {
        //print_help(); 
    }
    else if(argc == 1)
    {
        print_report_heading();
        struct dir_itr * dir_entry_itr1 = init_dir_itr(ROOT_DIR_SEC_INDEX);
    
        for(ditr_begin(dir_entry_itr1); !ditr_end(dir_entry_itr1); ditr_next(dir_entry_itr1))
        {
            struct dir_entry_info * current_entry = ditr_get(dir_entry_itr1);
            print_dir_entry_info(current_entry);
        }
        printf("--------------------------------------------------------------------------\n");
    }
    else
    {
        printf("\n%s%sERROR:%s Incorrect input. Please refer to \"lsr --help\"\n\n", T_BOLD, T_RED, T_RESET);
        return 0;
    }
    
    return 0;
    
}

