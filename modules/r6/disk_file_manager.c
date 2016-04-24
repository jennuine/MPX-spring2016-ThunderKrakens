#include <stdio.h>
#include "disk_img_manager.h"
#include "disk_file_manager.h"
#include "file_dir_iterator.h"

void type_file(struct dir_entry_info * file_entry_ptr)
{
    if (!file_entry_ptr || (file_entry_ptr->attributes & ATTR_ARCH_ONLY))
        return;
    
    int bytes_left = file_entry_ptr->file_size;
    struct file_itr * iter = init_file_itr(file_entry_ptr->first_log_clu);
    
    
    for (fitr_begin(iter); !fitr_end(iter) && bytes_left > 0; fitr_next(iter))
    {
        struct data_sector * curr_sec = fitr_get(iter);
        char data[512];
        //ch_arr_to_str(data, curr_sec->data, 512); this data is only depends on how it appear in the file.
        int i;
        for (i = 0; i < 512; i++)
        {
            
            data[i] = curr_sec->data[i];
            
            if (data[i] == '\n' && (i % 25 == 0))
            {
                while(!getchar());
            }
            //printf("%c", curr_sec->data[i]);
        }
        printf("%s", data);
    }
    
}

error_t extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path)
{
    if(!file_entry_ptr)
        return E_NULL_PTR;
    
    if(file_entry_ptr->attributes & ATTRIBUTE_SUBD)
        return E_INVPARA;
    
    FILE * out_file = fopen(out_file_path, "wb");
    if(!out_file)
        return E_INVPARA;
    
    int byte_left = file_entry_ptr->file_size;
    struct file_itr * fitr = init_file_itr(file_entry_ptr->first_log_clu);
    for(fitr_begin(fitr); !fitr_end(fitr) && byte_left > 0; fitr_next(fitr))
    {
        struct data_sector * curr_sec = fitr_get(fitr);
        if(byte_left > 512)
        {
            fwrite(curr_sec, 512, 1, out_file);
        }
        else
        {
            fwrite(curr_sec, byte_left, 1, out_file);
        }
        byte_left -= 512;
    }
    free(fitr);
    
    fclose(out_file);
    
    return E_NOERROR;
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

error_t import_file(const char * in_file_path, struct dir_entry_info * dest_dir)
{
    if(!dest_dir)
        return E_NULL_PTR; //ERROR Type 1
    
    if(!(dest_dir->attributes & ATTRIBUTE_SUBD))
        return E_INVPARA; //ERROR Type 2
        
    FILE * in_file = fopen(in_file_path, "rb");
    if(!in_file)
        return E_INVPARA; //ERROR Type 3
    
    int len = strlen(in_file_path);
    int temp = len - 1;
    for(; temp >= 0 && in_file_path[temp] != '/' && in_file_path[temp] != '\\'; temp--);
    temp++;
    if((len - temp) >= 13)
        return E_INVSTRF; //ERROR Type 4
    char file_full_name[13] = { 0 };
    memcpy(file_full_name, &in_file_path[temp], len - temp);
    
    str_to_upper_case(file_full_name, 13);
    
    if(get_entry_simple(dest_dir, file_full_name))
        return E_INVPARA; //ERROR Type 5
    
    uint32_t file_size = 0;
    if((file_size = get_import_file_size(in_file)) <= calc_free_space())
    {
        struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
        ditr_set_find_unused(dir_entry_itr);
        ditr_begin(dir_entry_itr);
        struct dir_entry_info * unused_entry = ditr_get(dir_entry_itr);
        free(dir_entry_itr);
        
        error_t errno;
        if((errno = seperate_file_name(file_full_name, unused_entry->file_name, unused_entry->extension)) != E_NOERROR)
            return errno; //ERROR Type 4
        
        unused_entry->attributes = ATTRIBUTE_ARCH;
        unused_entry->file_size = file_size;
        uint32_t byte_left = unused_entry->file_size;
        struct img_writer * fwriter = init_img_writer(unused_entry);
        struct data_sector temp_sec;
        for(; (int)byte_left > 0; )
        {
            memset(&temp_sec, 0, 512);
            fread(&temp_sec, 512, 1, in_file);
            iw_write(fwriter, &temp_sec);
            byte_left -= 512;
            //printf("*DEBUG* byte_left %d\n", byte_left);
        }
        free(fwriter);
        
    }
    fclose(in_file);
    //printf("*DEBUG*\n");
    return E_NOERROR;
}

error_t move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir)
{
    if(!file_entry || !dest_dir)
        return E_NULL_PTR; //ERROR Type 1
        
    if(file_entry->attributes & ATTRIBUTE_SUBD || !(dest_dir->attributes & ATTRIBUTE_SUBD))
        return E_INVPARA;
        
    char full_name[12];
    get_full_name(full_name, file_entry);
    if(get_entry_simple(dest_dir, full_name))
        return E_INVPARA; //ERROR Type 2
    
    struct dir_itr * dir_entry_itr = init_dir_itr(dest_dir->first_log_clu);
    ditr_set_find_unused(dir_entry_itr);
    ditr_begin(dir_entry_itr);
    struct dir_entry_info * unused_entry = ditr_get(dir_entry_itr);
    
    memcpy(unused_entry, file_entry, sizeof(struct dir_entry_info));
    file_entry->file_name[0] = 0xE5;
    
    free(dir_entry_itr);
    
    return E_NOERROR;
}