//#include "disk_img_manager.h"

struct dir_entry_info;

void type_file(struct dir_entry_info * file_entry_ptr);

void extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path);

void import_file(const char * in_file_path, struct dir_entry_info * dest_dir);

void move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir);