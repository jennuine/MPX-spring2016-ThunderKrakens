#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "ansi.h"
#include "file_dir_iterator.h"

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
            if(
            current_folder->file_array[i].file_name[0] != 0xE5 
            && current_folder->file_array[i].file_name[0] != 0
            && !(current_folder->file_array[i].attributes & ATTRIBUTE_HIDD)
            && !(current_folder->file_array[i].attributes & ATTRIBUTE_VOLL)
            )
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
    struct dir_itr * dir_entry_itr = init_dir_itr((folder_stack_top == 0 ? ROOT_DIR_SEC_INDEX : data_addr_to_sec_i(current_folder->file_array)));
    int i = 0;
    printf("\n");
    
    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        if((current_entry->attributes & ATTRIBUTE_SUBD))
        {
            i++;
            ch_arr_to_str(file_name, current_entry->file_name, 8);
            printf("%s%s%s/\t", T_DIR, file_name, T_DIR_OFF);
            if(i % 6)
                printf("\n");
        }
    }
    
    printf("\n\n");
    uint8_t attr_filter = ATTRIBUTE_SUBD | ATTRIBUTE_HIDD | ATTRIBUTE_VOLL;
    ditr_set_filter(dir_entry_itr, attr_filter);
    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        i++;
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        ch_arr_to_str(file_name, current_entry->file_name, 8);
        ch_arr_to_str(file_ext, current_entry->extension, 3);
        printf("%s.%s \t ", file_name, file_ext);
        if(!(i % 6))
            printf("\n");
    }
        
    printf("\n\n");
    free(dir_entry_itr);
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

/* We already had the same function above.
void ls()
{
    char file_name[10] = { 0 };
    char file_ext[5] = { 0 };
    int i = 0;
    if(folder_stack_top == 0)
    {
        for(i = 0; i < boot_sec->root_dir_max_num; i++)
        {
            if(current_folder->file_array[i].file_name[0] != 0xE5 
                && current_folder->file_array[i].file_name[0] != 0
                && !(current_folder->file_array[i].attributes & ATTRIBUTE_HIDD))
            {    
                if (current_folder->file_array[i].attributes & ATTRIBUTE_SUBD)
                { 
                    ch_arr_to_str(file_name, current_folder->file_array[i].file_name, 8);
                    printf("%s%s%s/\t", T_DIR, file_name, T_DIR_OFF);
                }    
                else if (!(current_folder->file_array[i].attributes & ATTRIBUTE_HIDD))
                {   
                    ch_arr_to_str(file_name, current_folder->file_array[i].file_name, 8);
                    ch_arr_to_str(file_ext, current_folder->file_array[i].extension, 3);
                    printf("%s.%s\t", file_name, file_ext);
                }
            }
        }
        printf("\n");
    }
}
*/

void rename_file(const char * old_name, const char * new_name)
{
    int i;
    char file_name[50];
    for(i = 0; i < boot_sec->root_dir_max_num; i++)
    {
        if(current_folder->file_array[i].file_name[0] != 0xE5 
            && current_folder->file_array[i].file_name[0] != 0
            && !(current_folder->file_array[i].attributes & ATTRIBUTE_HIDD))
        {
            ch_arr_to_str(file_name, current_folder->file_array[i].file_name, 8);
            if (!strcmp(file_name, old_name))
            {
                strcpy(current_folder->file_array[i].file_name, new_name); //Problem here.
                return;
            }
        }
    }
    
    printf("\t%sCould not locate file named: %s%s\n\n", T_ITCS, old_name, T_ITCS_OFF);
}