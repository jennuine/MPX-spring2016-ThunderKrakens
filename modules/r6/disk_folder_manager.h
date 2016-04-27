/**
 * @file disk_folder_manager.h
 * @author Thunder Krakens
 * @date April 27th, 2016
 * @brief Disk File Manager
 * @version R6
 *
 */

#define FOLDER_STACK_SIZE 1000

/**
 * @name folder_manager_init
 * @brief The initialization of folder manager
 * 
 */

void folder_manager_init();

/**
 * @name push_folder
 * @brief Push folder into the directory stack
 *
 * @param   child_folder_ptr     subfolders pointer
 * 
 */

void push_folder(struct dir_entry_info * child_folder_ptr);

/**
 * @name pop_folder
 * @brief Pop folder out of the directory stack
 * 
 */

void pop_folder();

/**
 * @name print_dir_entry_info
 * @brief Prints the directory's information to the user
 *
 * @param   folder_ptr     folders pointer
 * 
 */

void print_dir_entry_info(struct dir_entry_info * folder_ptr);

/**
 * @name list_dir_entry_report
 * @brief Reports the directory's entries
 * 
 */

void list_dir_entry_report();

/**
 * @name list_dir_entry_short
 * @brief Briefly, reports the directory's entries
 * 
 */

void list_dir_entry_short();

/**
 * @name print_dir_entry_info
 * @brief Prints the directory's information to the user
 *
 * @param   folder_ptr     folders pointer
 * 
 */

void print_curr_path();

error_t rename_entry(struct dir_entry_info * parent_dir_entry, struct dir_entry_info * file_entry, char * new_name);

struct dir_entry_info * get_entry_simple(const struct dir_entry_info * parent_dir_entry, char * nameStr);

struct dir_entry_info * get_entry(char * full_path);

void change_dir(char * full_path);

void list_files_entry_ext(const char * ext);

void list_files_entry_name(const char * name);

void list_file_report(const char *name, const char *ext, int wildcard);