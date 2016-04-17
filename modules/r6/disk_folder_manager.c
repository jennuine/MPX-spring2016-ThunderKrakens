#include "disk_img_manager.h"
#include "disk_folder_manager.h"

struct folder
{
    unsigned char folder_name[9];
    struct dir_entry_info * file_array;
};

struct folder * folder_stack[FOLDER_STACK_SIZE];

unsigned int folder_stack_top = 0;

struct folder * current_folder = NULL;



void folder_manager_init()
{
    current_folder = malloc(sizeof(struct folder));
    // strcpy(current_folder->folder_name, "root\0");
    
    char vol_label[12];
    ch_arr_to_str(vol_label, boot_sec->vol_label, 11);
    strcpy(current_folder->folder_name, vol_label);

    *root_dir_file_arr++;
    current_folder->file_array = root_dir_file_arr;
}

void push_folder(struct dir_entry_info * child_folder_ptr)
{
    if(folder_stack_top >= FOLDER_STACK_SIZE)
        return;
    
    folder_stack[folder_stack_top++] = current_folder;
    
    current_folder = malloc(sizeof(struct folder));
    ch_arr_to_str(current_folder->folder_name, child_folder_ptr->file_name, 8);
    //current_folder->file_array = child_folder_ptr;  //need to be finished later...
}

void pop_folder()
{
    if(folder_stack_top <= 0)
        return;
    
    free(current_folder);
    current_folder = folder_stack[--folder_stack_top];
}

void print_dir_entry_info(const struct dir_entry_info * folder_ptr)
{
    if(!folder_ptr)
        return;
    
    char attr_str[200] = { 0 };
    strcat(attr_str, (folder_ptr->attributes & ATTRIBUTE_READ) ? "|Read-Only\0" : "\0");
    strcat(attr_str, (folder_ptr->attributes & ATTRIBUTE_HIDD) ? "|Hidden\0" : "\0");
    strcat(attr_str, (folder_ptr->attributes & ATTRIBUTE_SYST) ? "|System\0" : "\0");
    strcat(attr_str, (folder_ptr->attributes & ATTRIBUTE_SUBD) ? "|Subdirectory\0" : "\0");
    char file_name[9] = { 0 };
    ch_arr_to_str(file_name, folder_ptr->file_name, 8);
    char file_ext[4] = { 0 };
    ch_arr_to_str(file_ext, folder_ptr->extension, 3);
    
    printf("File Name:           %s\n", file_name);
    printf("Extension:           %s\n", file_ext);
    printf("Attributes:          %s\n", attr_str);
    //we need more here...
    printf("First Log Cluster:   %d\n", folder_ptr->first_log_clu);
    printf("File Size:           %d\n", folder_ptr->file_size);
    printf("\n");
}

void print_curr_dir_entry_list()
{
    //need to be corrected later...
    int i = 0;
    if(folder_stack_top == 0)
    {//if it is in the root folder.
        for(i = 0; i < boot_sec->root_dir_max_num; i++)
        {
            if(current_folder->file_array[i].file_name[0] != 0xE5 && current_folder->file_array[i].file_name[0] != 0)
                if (!(current_folder->file_array[i].attributes & 0x02))
                    print_dir_entry_info(&(current_folder->file_array[i]));
        }
    }
    else
    {//else it is in any subdirectory
        
    }
}

void list_curr_file_and_dir()
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    int i = 0;
    printf("\n");
    printf("Directories: \n");
    if(folder_stack_top == 0)
    {//if it is in the root folder.
        for(i = 0; i < boot_sec->root_dir_max_num; i++)
        {
            if(current_folder->file_array[i].file_name[0] != 0xE5 && current_folder->file_array[i].file_name[0] != 0 && (current_folder->file_array[i].attributes & ATTRIBUTE_SUBD))
            {
                if (!(current_folder->file_array[i].attributes & 0x02))
                {
                    ch_arr_to_str(file_name, current_folder->file_array[i].file_name, 8);
                    printf("%s \t\t ", file_name);
                }
            }
        }
    }
    else
    {//else it is in any subdirectory
        
    }
    printf("\n\n");
    
    printf("Files: \n");
    if(folder_stack_top == 0)
    {//if it is in the root folder.
        for(i = 0; i < boot_sec->root_dir_max_num; i++)
        {
            if(current_folder->file_array[i].file_name[0] != 0xE5 && current_folder->file_array[i].file_name[0] != 0 && !(current_folder->file_array[i].attributes & ATTRIBUTE_SUBD))
            {
                if (!(current_folder->file_array[i].attributes & 0x02) && !(current_folder->file_array[i].attributes & 0x10 ))
                {
                    ch_arr_to_str(file_name, current_folder->file_array[i].file_name, 8);
                    ch_arr_to_str(file_ext, current_folder->file_array[i].extension, 3);
                    printf("%s.%s \t\t ", file_name, file_ext);
                }
            }
        }
    }
    else
    {//else it is in any subdirectory
        
    }
    printf("\n\n");
}

void print_curr_path()
{
    char dir_name[9] = { 0 };
    int i = 0;
    for(i = 0; i < folder_stack_top; i++)
    {
        printf("%s\\", folder_stack[i]->folder_name);
    }
    printf("%s\\", current_folder->folder_name);
}