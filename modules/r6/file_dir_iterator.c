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
    
    //Attributes Filter (mask). "1" means do not contain in the iteration
    uint8_t attr_filter;
    //want_unused (boolean). "1" means finding an unused entry.
    uint8_t want_unused;
}
);

PACKED(
struct img_writer
{
    struct dir_entry_info * entry_ptr;
    uint16_t curr_sec_i;
}    
);
/*
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
*/
//True means this entry needs to be filted out.
static uint8_t ditr_filter(struct dir_itr * itr_ptr)
{
    struct dir_entry_info * curr_entry = ditr_get(itr_ptr);
    
    return
    (
        curr_entry 
        && 
        (
            (//Normal iteration, find the used entry.
                !itr_ptr->want_unused
                && 
                (
                    curr_entry->file_name[0] == 0xE5 
                    || curr_entry->file_name[0] == 0
                    || (curr_entry->attributes & itr_ptr->attr_filter)
                )
            )
            || 
            (//Find unused entry.
                itr_ptr->want_unused 
                &&
                (
                    curr_entry->file_name[0] != 0xE5 
                    && curr_entry->file_name[0] != 0
                )
            )
        )
        
    );
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
    //ditr_apply_filter(result);
    if(ditr_filter(result))
        ditr_next(result);
        
    return result;
}

void ditr_set_filter(struct dir_itr * itr_ptr, uint8_t attr_filter)
{
    if(!itr_ptr)
        return;
    itr_ptr->attr_filter = attr_filter;
}

void fitr_next_offset(struct file_itr * itr_ptr)
{
    if(!itr_ptr || fitr_end(itr_ptr))
        return;
        
    fat(&itr_ptr->curr_sec_i, itr_ptr->curr_sec_i);
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
    
    //ditr_apply_filter(itr_ptr);
    if(ditr_filter(itr_ptr))
        ditr_next(itr_ptr);
}

void ditr_set_find_unused(struct dir_itr * itr_ptr)
{
    if(!itr_ptr)
        return;
    
    itr_ptr->want_unused = 1;
}

uint8_t fitr_end(struct file_itr * itr_ptr)
{
    if(!itr_ptr)
        return 1;
        
    return (0xFF8 <= itr_ptr->curr_sec_i && itr_ptr->curr_sec_i <= 0xFFF);
}

uint8_t ditr_end(struct dir_itr * itr_ptr)
{
    if(!itr_ptr)
        return 1;
        
    
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
    }
    else
    {
        itr_ptr->curr_entry_i++;
        if(itr_ptr->curr_entry_i >= 16)
        {
            fat(&itr_ptr->curr_sec_i, itr_ptr->curr_sec_i);
            itr_ptr->curr_entry_i = 0;
        }
        
    }
    
    if(ditr_filter(itr_ptr))
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

struct img_writer * init_img_writer(struct dir_entry_info * entry_ptr)
{
    struct img_writer * result = malloc(sizeof(struct img_writer));
    
    result->entry_ptr = entry_ptr;
    result->curr_sec_i = 0;
    
    return result;
}

void iw_write(struct img_writer * writer_ptr, const struct data_sector * data)
{
    uint16_t unuse_sec_i = find_unused_fat();
    
    //uint16_t test1 = 0;
    //fat(&test1, unuse_sec_i);
    //printf("*DEBUG* %#x has %#x\n", unuse_sec_i, test1);
    
    write_fat(0xFFF, unuse_sec_i);
    memcpy(get_data_ptr(unuse_sec_i), data, 512);
    
    //fat(&test1, unuse_sec_i);
    //printf("*DEBUG* Wrote @ %#x\n", unuse_sec_i);
    //printf("*DEBUG* %#x has %#x\n", unuse_sec_i, test1);
    
    if(writer_ptr->curr_sec_i < 2)
    {//First sector
        writer_ptr->entry_ptr->first_log_clu = unuse_sec_i;
    }
    else
    {
        write_fat(unuse_sec_i, writer_ptr->curr_sec_i);
        //fat(&test1, writer_ptr->curr_sec_i);
        //printf("*DEBUG* Last fat point to @ %#x\n", test1);
    }
    writer_ptr->curr_sec_i = unuse_sec_i;
    //printf("*DEBUG*\n");
}