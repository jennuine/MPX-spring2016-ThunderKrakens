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
    //printf("\n*debug* child_folder_ptr @: %#X\n", child_folder_ptr);
    if(folder_stack_top >= FOLDER_STACK_SIZE)
        return;
    if(!child_folder_ptr || !(child_folder_ptr->attributes & ATTRIBUTE_SUBD))
        return;
    
    folder_stack[folder_stack_top++] = current_folder;
    
    current_folder = malloc(sizeof(struct folder));
    ch_arr_to_str(current_folder->folder_name, child_folder_ptr->file_name, 8);
    //current_folder->file_array = get_data_ptr(child_folder_ptr->first_log_clu);
    current_folder->log_sec_index = child_folder_ptr->first_log_clu;
    
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
    char write_date[11] = { 0 };
    char write_time[11] = { 0 };
    ch_arr_to_str(file_name, entry_ptr->file_name, 8);
    ch_arr_to_str(file_ext, entry_ptr->extension, 3);
    get_fat_date_str(write_date, entry_ptr->last_wri_date);
    get_fat_time_str(write_time, entry_ptr->last_wri_time);
    
    printf
    (
        "|%s%-8s%s|%-3s|%s|%s|%s|%-7u|%-7d|\n",
        (entry_ptr->attributes & ATTRIBUTE_SUBD) ? T_DIR : "",
        file_name,
        (entry_ptr->attributes & ATTRIBUTE_SUBD) ? T_DIR_OFF : "",
        file_ext,
        get_attr_str(entry_ptr->attributes),
        write_date,
        write_time,
        entry_ptr->first_log_clu,
        entry_ptr->file_size
    );

}

void list_dir_entry_report()
{
    printf("--------------------------------------------------------------------------\n");
    printf
    (
        "|%-8s|%-3s|%-25s|%-8s|%-8s|%-7s|%-7s|\n",
        "name\0",
        "ext\0",
        "attributes\0",
        "wri date\0",
        "wri time\0",
        "sec num\0",
        "size\0"
    );
    
    printf("|--------|---|-------------------------|--------|--------|-------|-------|\n");
    
    struct dir_itr * dir_entry_itr1 = init_dir_itr(current_folder->log_sec_index);

    for(ditr_begin(dir_entry_itr1); !ditr_end(dir_entry_itr1); ditr_next(dir_entry_itr1))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr1);
        if (current_entry->file_name[0] != '.')
            print_dir_entry_info(current_entry);
    }
    
    free(dir_entry_itr1);
    
    printf("--------------------------------------------------------------------------\n");
}

void list_dir_entry_short()
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    struct dir_itr * dir_entry_itr = init_dir_itr(current_folder->log_sec_index);
    int i = 0;

    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        if((current_entry->attributes & ATTRIBUTE_SUBD) && current_entry->file_name[0] != '.')
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

void list_files_entry_ext(const char * ext)
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    
    struct dir_itr * dir_entry_itr = init_dir_itr(current_folder->log_sec_index);
    int i;
    uint8_t attr_filter = ATTRIBUTE_SUBD | ATTRIBUTE_HIDD | ATTRIBUTE_VOLL;
    ditr_set_filter(dir_entry_itr, attr_filter);
    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        i++;
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        ch_arr_to_str(file_ext, current_entry->extension, 3);
        if (!strcmp(ext, file_ext))
        {
            ch_arr_to_str(file_name, current_entry->file_name, 8);
            sprintf(full_name, "%s.%s", file_name, file_ext);
            printf("%-15s", full_name);
            
            if(!(i % 5))
                printf("\n");
        }    
    }
    printf("\n\n");
    free(dir_entry_itr);
    
    if (strlen(full_name) == 0)
    {
        printf("\n%s%sERROR:%s could not find files with the extension \'%s\'.\n\n", T_BOLD, T_RED, T_RESET, ext);
    }

}

void list_files_entry_name(const char * name)
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    
    struct dir_itr * dir_entry_itr = init_dir_itr(current_folder->log_sec_index);
    int i;
    uint8_t attr_filter = ATTRIBUTE_SUBD | ATTRIBUTE_HIDD | ATTRIBUTE_VOLL;
    ditr_set_filter(dir_entry_itr, attr_filter);
    
    for(ditr_begin(dir_entry_itr), i = 0; !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        i++;
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        ch_arr_to_str(file_name, current_entry->file_name, 8);
        
        if (!strcmp(name, file_name))
        {
            ch_arr_to_str(file_ext, current_entry->extension, 3);
            sprintf(full_name, "%s.%s", file_name, file_ext);
            printf("%-15s", full_name);
            
            if(!(i % 5))
                printf("\n");
        }    
    }
    printf("\n\n");
    free(dir_entry_itr);
    
    if (strlen(full_name) == 0)
    {
        printf("\n%s%sERROR:%s could not find files with the name \'%s\'.\n\n", T_BOLD, T_RED, T_RESET, name);
    }

}

void print_report_heading()
{
    printf("--------------------------------------------------------------------------\n");
    printf
    (
        "|%-8s|%-3s|%-25s|%-8s|%-8s|%-7s|%-7s|\n",
        "name\0",
        "ext\0",
        "attributes\0",
        "wri date\0",
        "wri time\0",
        "sec num\0",
        "size\0"
    );
    
    printf("|--------|---|-------------------------|--------|--------|-------|-------|\n");
}

void list_file_report(const char *name, const char *ext, int wildcard)
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    struct dir_itr * dir_entry_itr1 = init_dir_itr(current_folder->log_sec_index);
    ditr_set_filter(dir_entry_itr1, 0);
    int flag = 0;
    
    for(ditr_begin(dir_entry_itr1); !ditr_end(dir_entry_itr1); ditr_next(dir_entry_itr1))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr1);
        
        ch_arr_to_str(file_name, current_entry->file_name, 8);
        ch_arr_to_str(file_ext, current_entry->extension, 3);
        
        if (!wildcard && !strcmp(file_name, name) && (!strcmp(file_ext, ext) || !strcmp(file_ext, "")))
        {
            if (flag == 0) 
                print_report_heading();
            
            print_dir_entry_info(current_entry);
            flag = 1;
        } else if ( wildcard ) {
            if ( !strcmp(file_ext, ext) && !strcmp(name, "*") ) 
            {
                if (flag == 0)
                    print_report_heading();
                    
                print_dir_entry_info(current_entry);
                flag = 1;
            }
            else if (!strcmp(file_name, name) && !strcmp(ext, "*"))
            {
                if (flag == 0)
                    print_report_heading();
                    
                print_dir_entry_info(current_entry);
                flag = 1;
            }
        }
        

    }
    
    free(dir_entry_itr1);
    
    if (flag)
        printf("--------------------------------------------------------------------------\n");
    else
        printf("\n%s%sERROR:%s could not find file named \'%s.%s\'\n\n", T_BOLD, T_RED, T_RESET, name, ext);
}



void print_curr_path()
{
    char dir_name[9] = { 0 };
    int i = 0;
    for(i = 0; i < folder_stack_top; i++)
    {
        printf("%s/", folder_stack[i]->folder_name);
    }
    printf("%s/", current_folder->folder_name);
}

error_t rename_entry(struct dir_entry_info * parent_dir_entry, struct dir_entry_info * file_entry, char * new_name)
{
    if (parent_dir_entry == NULL || file_entry == NULL)
    {
        return E_NULL_PTR;
    }
    str_to_upper_case(new_name, strlen(new_name));
    if(get_entry_by_name(parent_dir_entry, new_name))
        return E_INVPARA;
        
    error_t errno = E_NOERROR;
    
    errno = validate_filename(new_name);
    
    if(errno != E_NOERROR)
        return errno;
    
    str_to_upper_case(new_name, strlen(new_name));
    return seperate_file_name(new_name, file_entry->file_name, file_entry->extension);
}

struct dir_entry_info * get_entry_by_name(const struct dir_entry_info * parent_dir_entry, char * nameStr)
{
    if(parent_dir_entry && !(parent_dir_entry->attributes & ATTRIBUTE_SUBD))
        return NULL;
    char full_name[12] = { 0 };
    
    str_to_upper_case(nameStr, strlen(nameStr));
    
    uint16_t sec_index = current_folder->log_sec_index;
    
    if(parent_dir_entry)
    {
        sec_index = parent_dir_entry->first_log_clu;
    }
    
    struct dir_itr * dir_entry_itr = init_dir_itr(sec_index);
    
    for(ditr_begin(dir_entry_itr); !ditr_end(dir_entry_itr); ditr_next(dir_entry_itr))
    {
        struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
        
        get_full_name(full_name, current_entry);
        
        if(!strcmp(full_name, nameStr))
            return current_entry;
    }
    
    free(dir_entry_itr);
    return NULL;
}

struct dir_entry_info * get_entry(char * full_path)
{
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    char full_name[12] = { 0 };
    
    str_to_upper_case(full_path, strlen(full_path));

    char * simple_path = strtok(full_path, "\\/");
    uint16_t curr_sec_i = current_folder->log_sec_index;
    struct dir_entry_info * result = NULL;
    
    if(curr_sec_i == ROOT_DIR_SEC_INDEX)
    {
        result = root_dir_entry;
    }
    else
    {
        result = get_data_ptr(curr_sec_i);
    }
    //printf("*DEBUG* get_entry, root_dir_file_arr:%#x \n", root_dir_file_arr);
    
    while(simple_path)
    {
        unsigned char isFound = 0;
        if(!strcmp(simple_path, "."))
        {
            isFound = 1;
        }
        else
        {
            struct dir_itr * dir_entry_itr = init_dir_itr(curr_sec_i);
            for(ditr_begin(dir_entry_itr); !ditr_end(dir_entry_itr) && !isFound; ditr_next(dir_entry_itr))
            {
                struct dir_entry_info * current_entry = ditr_get(dir_entry_itr);
                ch_arr_to_str(file_name, current_entry->file_name, 8);
                ch_arr_to_str(file_ext, current_entry->extension, 3);
                if(strlen(file_ext) > 0)
                    sprintf(full_name, "%s.%s", file_name, file_ext);
                else
                    sprintf(full_name, "%s", file_name);
                if(!strcmp(full_name, simple_path))
                {
                    result = current_entry;
                    curr_sec_i = current_entry->first_log_clu;
                    isFound = 1;
                }
            }
            free(dir_entry_itr);
        }
        simple_path = strtok(NULL, "\\/");
        result = isFound ? result : NULL;
    }
    //printf("*DEBUG* get_entry, return:%#x \n", result);
    return result;
}

void change_dir(char * full_path)
{
    char * simple_path = strtok(full_path, "\\/");
    while(simple_path)
    {   //printf("\n*debug* simple_path: %s\n", simple_path);
        if(!strcmp(simple_path, "."))
        {
            
        }
        else if(!strcmp(simple_path, ".."))
        {
            pop_folder();
        }
        else
            push_folder(get_entry_by_name(NULL, simple_path));
            
        simple_path = strtok(NULL, "\\/");
    }
}