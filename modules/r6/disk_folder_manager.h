/**
 * @file disk_folder_manager.h
 * @author Thunder Krakens
 * @date April 27th, 2016
 * @brief Disk Folder Manager
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
 * @brief Briefly reports the directory's entries
 * 
 */

void list_dir_entry_short();

/**
 * @name print_curr_path
 * @brief Prints current directory's information to the user
 * 
 */

void print_curr_path();

/**
 * @name rename_entry
 * @brief Rename current entry's name
 *
 * @param   parent_dir_entry    The name of the parent's directory
 * @param   file_entry          Entry's file
 * @param   new_name            The new name of the entry
 * 
 */

error_t rename_entry(struct dir_entry_info * parent_dir_entry, struct dir_entry_info * file_entry, char * new_name);

/**
 * @name get_entry_simple
 * @brief Briefly prints the entry's information to the user
 *
 * @param   parent_dir_entry    The name of the parent's directory
 * @param   nameStr             Convert name to a string
 * 
 */

struct dir_entry_info * get_entry_simple(const struct dir_entry_info * parent_dir_entry, char * nameStr);

/**
 * @name get_entry
 * @brief Prints the entry's full path to the user
 *
 * @param   full_path    Retrieve the full path
 * 
 */

struct dir_entry_info * get_entry(char * full_path);

/**
 * @name change_dir
 * @brief Change the directory
 *
 * @param  full_path    Retrieve the full path
 * 
 */

void change_dir(char * full_path);

/**
 * @name list_files_entry_ext
 * @brief Lists the extension of entries
 *
 * @param   ext   extension of entry
 * 
 */

void list_files_entry_ext(const char * ext);

/**
 * @name list_files_entry_name
 * @brief Lists the name of entries
 *
 * @param  name     name of entry
 * 
 */

void list_files_entry_name(const char * name);

/**
 * @name list_file_report
 * @brief Lists the properties of the files
 *
 * @param   name        name of file
 * @param   ext         extension of file
 * @param   wildcard      
 * 
 */

void list_file_report(const char *name, const char *ext, int wildcard);