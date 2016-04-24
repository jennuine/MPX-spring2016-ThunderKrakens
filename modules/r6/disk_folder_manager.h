#define FOLDER_STACK_SIZE 1000

void folder_manager_init();

void push_folder(struct dir_entry_info * child_folder_ptr);

void pop_folder();

void print_dir_entry_info(struct dir_entry_info * folder_ptr);

void list_dir_entry_report();

void list_dir_entry_short();

void print_curr_path();

error_t rename_entry(struct dir_entry_info * parent_dir_entry, struct dir_entry_info * file_entry, char * new_name);

struct dir_entry_info * get_entry_simple(const struct dir_entry_info * parent_dir_entry, char * nameStr);

struct dir_entry_info * get_entry(char * full_path);

void change_dir(char * full_path);