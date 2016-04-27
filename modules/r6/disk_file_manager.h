//#include "disk_img_manager.h"

struct dir_entry_info;

void type_file(struct dir_entry_info * file_entry_ptr);

error_t extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path);

error_t import_file(const char * in_file_path, struct dir_entry_info * dest_dir);

error_t move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir);