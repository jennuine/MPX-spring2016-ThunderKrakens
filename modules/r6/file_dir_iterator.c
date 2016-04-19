#include "file_dir_iterator.h"
#include "disk_img_manager.h"
#include <stdlib.h>
#include "../packing.h"

PACKED(
struct file_itr
{
    uint16_t ori_sec_i;
    uint16_t curr_sec_i;
    
}
);

PACKED(
struct dir_itr
{
    uint16_t ori_sec_i;
    uint16_t curr_sec_i;
    
    uint8_t curr_entry_i;
    
    //Attributes Filter. "1" means do not contain in the iteration
    uint8_t attr_filter;
}
);

static void ditr_apply_filter(struct dir_itr * itr_ptr)
{
    struct dir_entry_info * curr_entry = ditr_get(itr_ptr);
    
    while
    (
        curr_entry && 
        (
            curr_entry->file_name[0] == 0xE5 
            || curr_entry->file_name[0] == 0
            || (curr_entry->attributes & itr_ptr->attr_filter)
        )
    )
    {
        ditr_next(itr_ptr);
        curr_entry = ditr_get(itr_ptr);
    }
}

struct file_itr * init_file_itr(const uint16_t sec_index)
{
    struct file_itr * result = malloc(sizeof(struct file_itr));
        
    result->ori_sec_i = sec_index;
    result->curr_sec_i = sec_index;
    
    return result;
}

struct dir_itr * init_dir_itr(const uint16_t sec_index)
{
    struct dir_itr * result = malloc(sizeof(struct dir_itr));
        
    result->ori_sec_i = sec_index;
    result->curr_sec_i = sec_index;
    
    result->curr_entry_i = 0;
    
    result->attr_filter = ATTRIBUTE_HIDD | ATTRIBUTE_VOLL;
    ditr_apply_filter(result);
    return result;
}

void ditr_set_filter(struct dir_itr * itr_ptr, uint8_t attr_filter)
{
    if(!itr_ptr)
        return;
    itr_ptr->attr_filter = attr_filter;
}

void fitr_begin(struct file_itr * itr_ptr)
{
    if(!itr_ptr)
        return;
        
    itr_ptr->curr_sec_i = itr_ptr->ori_sec_i;
}

void ditr_begin(struct dir_itr * itr_ptr)
{
    if(!itr_ptr)
        return;
           
    itr_ptr->curr_sec_i = itr_ptr->ori_sec_i;
    itr_ptr->curr_entry_i = 0;
    
    ditr_apply_filter(itr_ptr);
}

uint8_t fitr_end(struct file_itr * itr_ptr)
{
    if(!itr_ptr)
        return 1;
        
    return (0xFF8 <= itr_ptr->curr_sec_i && itr_ptr->curr_sec_i <= 0xFFF);
}

uint8_t ditr_end(struct dir_itr * itr_ptr)
{//printf("*debug: %lu\n", (uint64_t)itr_ptr);
    if(!itr_ptr)
        return 1;
        
    //printf("*debug: %X\n", itr_ptr->curr_sec_i);
    return (0xFF8 <= itr_ptr->curr_sec_i && itr_ptr->curr_sec_i <= 0xFFF);
}

void fitr_next(struct file_itr * itr_ptr)
{
    if(!itr_ptr || fitr_end(itr_ptr))
        return;
        
    fat(&itr_ptr->curr_sec_i, itr_ptr->curr_sec_i);
}

void ditr_next(struct dir_itr * itr_ptr)
{
    if(!itr_ptr || ditr_end(itr_ptr))
        return;
        
    if(itr_ptr->ori_sec_i == ROOT_DIR_SEC_INDEX)
    {
        itr_ptr->curr_entry_i++;
        if(itr_ptr->curr_entry_i >= boot_sec->root_dir_max_num)
            itr_ptr->curr_sec_i = 0xFFF;
        //printf("*debug: %d\n", itr_ptr->curr_entry_i);
    }
    else
    {
        itr_ptr->curr_entry_i++;
        if(itr_ptr->curr_entry_i >= 16)
        {
            //itr_ptr->curr_sec_i = 0xFFF;
            fat(&itr_ptr->curr_sec_i, itr_ptr->curr_sec_i);
            itr_ptr->curr_entry_i = 0;
        }
        
    }
    
    struct dir_entry_info * curr_entry = ditr_get(itr_ptr);
    
    if
    (
        curr_entry && 
        (
            curr_entry->file_name[0] == 0xE5 
            || curr_entry->file_name[0] == 0
            || (curr_entry->attributes & itr_ptr->attr_filter)
        )
    )
    {
        ditr_next(itr_ptr);
    }
}

struct data_sector * fitr_get(struct file_itr * itr_ptr)
{
    if(!itr_ptr || fitr_end(itr_ptr))
        return NULL;
        
    return get_data_ptr(itr_ptr->curr_sec_i);
}

struct dir_entry_info * ditr_get(struct dir_itr * itr_ptr)
{
    if(!itr_ptr || ditr_end(itr_ptr))
        return NULL;
    
    if(itr_ptr->ori_sec_i == ROOT_DIR_SEC_INDEX)
    {
        return &root_dir_file_arr[itr_ptr->curr_entry_i];
    }
    
    struct dir_entry_info * first_entry = get_data_ptr(itr_ptr->curr_sec_i);
    
    return &first_entry[itr_ptr->curr_entry_i];
}
