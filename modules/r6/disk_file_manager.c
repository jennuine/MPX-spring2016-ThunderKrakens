#include "disk_img_manager.h"
#include "disk_file_manager.h"
#include "file_dir_iterator.h"

void type_file(struct dir_entry_info * file_entry_ptr)
{
    
}

void extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path)
{
    if(!file_entry_ptr || file_entry_ptr->attributes & ATTRIBUTE_SUBD)
        return;
        
    FILE * out_file = fopen(out_file_path, "wb");
    if(!out_file)
        return;
    
    uint32_t byte_left = file_entry_ptr->file_size;
    struct file_itr * fitr = init_file_itr(file_entry_ptr->first_log_clu);
    for(fitr_begin(fitr); !fitr_end(fitr) && byte_left > 0; fitr_next(fitr))
    {
        struct data_sector * curr_sec = fitr_get(fitr);
        if(byte_left > 512)
        {
            fwrite(curr_sec, 512, 1, out_file);
            byte_left -= 512;
        }
        else
        {
            fwrite(curr_sec, byte_left, 1, out_file);
            byte_left -= 0;
        }
    }
    
    free(fitr);
    fclose(out_file);
}


void import_file(const char * in_file_path, struct dir_entry_info * dest_dir)
{
    struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
    ditr_set_find_unused(dir_entry_itr);
    ditr_begin(dir_entry_itr);
    
}

void move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir)
{
    struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
    ditr_set_find_unused(dir_entry_itr);
    ditr_begin(dir_entry_itr);
    
}