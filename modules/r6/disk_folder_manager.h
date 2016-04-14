#define FOLDER_STACK_SIZE 1000

void folder_manager_init();

void push_folder(struct dir_entry_info * child_folder_ptr);

void pop_folder();

void print_dir_entry_info(const struct dir_entry_info * folder_ptr);

void print_curr_dir_entry_list();

void print_curr_path();