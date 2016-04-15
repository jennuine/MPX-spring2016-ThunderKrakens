#include "disk_img_manager.h"
#include "disk_folder_manager.h"

int main(int argc, char **argv)
{
    
    // load_image_file("imgs/winb98se.IMA");
    
    if (argv[0] == NULL) 
    {
        printf("Please enter the image filepath.\n");
        return 0;
    }
    
    
    
    load_image_file(argv[1]);
    folder_manager_init();
    
    print_curr_dir_entry_list();
    list_curr_file_and_dir();
    char * command_str;
    size_t command_str_len = 0;
    int is_run = 1;
    while(is_run)
    {
        print_curr_path();
        printf("> ");
        getline(&command_str, &command_str_len, stdin);
        int i = 0;
        for(i = 0; i < command_str_len; i++)
        {
            if(isspace(command_str[i]) && command_str[i] != ' ')
            {
                command_str[i] = '\0';
            }
        }
        
        if(!strcmp(command_str, "exit"))
        {
            is_run = 0;
        }
        
    }
    
    free(command_str);
    command_str = NULL;
    clean_buffers();
    return 0;
}

#if 0

//things we still need:
//      print boot sec info
//      print root dir
//      change directory
//      list dir
//      Type
//      Rename
//Extra Credit:
//      Move
//      Adding file

#endif 