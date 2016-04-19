#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "ansi.h"
#include "file_dir_iterator.h"

#include "../packing.h"

PACKED(
struct folder
{
    unsigned char folder_name[9];
    uint16_t log_sec_index;
}
);

struct folder * folder_stack[FOLDER_STACK_SIZE];

unsigned int folder_stack_top = 0;

struct folder * current_folder = NULL;



void folder_manager_init()
{
    current_folder = malloc(sizeof(struct folder));
    
    char vol_label[12];
    ch_arr_to_str(vol_label, boot_sec->vol_label, 11);
    strcpy(current_folder->folder_name, vol_label);

    current_folder->log_sec_index = ROOT_DIR_SEC_INDEX;
}

void push_folder(struct dir_entry_info * child_folder_ptr)
{
    if(folder_stack_top >= FOLDER_STACK_SIZE)
        return;
    if(!child_folder_ptr || !(child_folder_ptr->attributes & ATTRIBUTE_SUBD))
        return;
    
    folder_stack[folder_stack_top++] = current_folder;
    
    current_folder = malloc(sizeof(struct folder));
    ch_arr_to_str(current_folder->folder_name, child_folder_ptr->file_name, 8);
    //current_folder->file_array = get_data_ptr(child_folder_ptr->first_log_clu);
    current_folder->log_sec_index = child_folder_ptr->first_log_clu;
    //printf("\n*debug* file_array @: %X\n", current_folder->file_array);
}

void pop_folder()
{
    if(folder_stack_top <= 0)
        return;
    
    free(current_folder);
    current_folder = folder_stack[--folder_stack_top];
}

static const char * get_attr_str(uint8_t attributes)
{
    static char result[30];
    sprintf(result, 
    "/%s/%s/%s/%s/%s/%s/", 
    (attributes & ATTRIBUTE_READ ? "RED" : "   "),
    (attributes & ATTRIBUTE_HIDD ? "HID" : "   "),
    (attributes & ATTRIBUTE_SYST ? "SYS" : "   "),
    (attributes & ATTRIBUTE_VOLL ? "VOL" : "   "),
    (attributes & ATTRIBUTE_SUBD ? "SUB" : "   "),
    (attributes & ATTRIBUTE_ARCH ? "ARC" : "   ")
    );
    return result;
}

void print_dir_entry_info(struct dir_entry_info * entry_ptr)
{
    if(!entry_ptr)
        return;
    
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    ch_arr_to_str(file_name, entry_ptr->file_name, 8);
    ch_arr_to_str(file_ext, entry_ptr->extension, 3);
    
    
    printf
    (
        "| %-8s | %-3s | %s | %-7u | %-7u |\n",
        file_name,
        file_ext,
        get_attr_str(entry_ptr->attributes),
        entry_ptr->first_log_clu,
        entry_ptr->file_size
    );
    
    
}

void list_dir_entry_report()
{
    printf
    (
        "| %-8s | %-3s | %-25s | %-7s | %-7s |\n",
        "name\0",
        "ext\0",
        "attributes\0",
        "sec num\0",
        "size\0"
    );
    struct dir_itr * dir_entry_itr1 = init_dir_itr(current_folder->log_sec_index);
    ditr_set_filter(dir_entry_itr1, 0);
    for(ditr_begin(dir_entry_itr1); !ditr_end(dir_entry_itr1); ditr_next(dir_entry_itr1))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr1);
        print_dir_entry_info(current_entry);
    }
    
    free(dir_entry_itr1);
    printf("\n");
}

void list_dir_entry_short()
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    struct dir_itr * dir_entry_itr = init_dir_itr(current_folder->log_sec_index);
    int i = 0;
    printf("\n");
    
    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        if((current_entry->attributes & ATTRIBUTE_SUBD))
        {
            i++;
            ch_arr_to_str(file_name, current_entry->file_name, 8);
            sprintf(full_name, "%s/", file_name);
            printf("%s%-15s%s", T_DIR, full_name, T_DIR_OFF);
            if(!(i % 5))
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
        sprintf(full_name, "%s.%s", file_name, file_ext);
        printf("%-15s", full_name);
        if(!(i % 5))
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

void rename_entry(struct dir_entry_info * folder_ptr, const char * new_name)
{
    if(0 < strlen(new_name) && strlen(new_name) <= 8)
    {
        str_to_ch_arr(folder_ptr->file_name, new_name, 8);
    }
    else
    {
       printf("\t%s The length of the filename must within the range of [1, 8]. %s\n\n", T_ITCS, T_ITCS_OFF); 
    }
}

struct dir_entry_info * get_entry(const char * nameStr)
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    
    struct dir_itr * dir_entry_itr = init_dir_itr(current_folder->log_sec_index);
    
    for(ditr_begin(dir_entry_itr); !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        ch_arr_to_str(file_name, current_entry->file_name, 8);
        ch_arr_to_str(file_ext, current_entry->extension, 3);
        if(strlen(file_ext) > 0)
            sprintf(full_name, "%s.%s", file_name, file_ext);
        else
            sprintf(full_name, "%s", file_name);
        if(!strcmp(full_name, nameStr))
            return current_entry;
    }
    
    free(dir_entry_itr);
    return NULL;
}