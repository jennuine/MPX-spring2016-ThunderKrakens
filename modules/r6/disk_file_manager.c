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

static const size_t get_import_file_size(FILE * file_stream)
{
    if(!file_stream)
        return 0;
    size_t size = 0;
    for(fseek(file_stream, 0, SEEK_SET); fgetc(file_stream) != EOF; size++);
    fseek(file_stream, 0, SEEK_SET);
    return size;
}

void import_file(const char * in_file_path, struct dir_entry_info * dest_dir)
{
    if(!dest_dir || !(dest_dir->attributes & ATTRIBUTE_SUBD))
        return;
        
    FILE * in_file = fopen(in_file_path, "rb");
    if(!in_file)
        return;
    
    
    
    struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
    ditr_set_find_unused(dir_entry_itr);
    ditr_begin(dir_entry_itr);
    struct dir_entry_info * unused_entry = ditr_get(dir_entry_itr);
    
    
    
    free(dir_entry_itr);
}

void move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir)
{
    if(!file_entry || !dest_dir || file_entry->attributes & ATTRIBUTE_SUBD || !(dest_dir->attributes & ATTRIBUTE_SUBD))
        return;
        
    struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
    ditr_set_find_unused(dir_entry_itr);
    ditr_begin(dir_entry_itr);
    struct dir_entry_info * unused_entry = ditr_get(dir_entry_itr);
    
    memcpy(unused_entry, file_entry, sizeof(struct dir_entry_info));
    file_entry->file_name[0] = 0xE5;
    
    free(dir_entry_itr);
}