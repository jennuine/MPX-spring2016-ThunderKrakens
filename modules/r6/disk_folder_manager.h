/**
 * @file disk_folder_manager.h
 * @author Thunder Krakens
 * @date April 28th, 2016
 * @brief Disk Folder Manager
 * 
 * This contains functions that are related to the directory/folder.
 * @version R6
 */

#define FOLDER_STACK_SIZE 1000

/**
 * @name folder_manager_init
 * @brief Initializes the folder manager
 * 
 * Initializes the folder stack. Reads the boot sector's volume label and sets it as the root folder.
 */
void folder_manager_init();

/**
 * @name push_folder
 * @brief Pushes the current folder onto the folder stack
 *
 * Pushes current onto the folder stack and sets the child folder as the current folder.
 * 
 * @param   child_folder_ptr     the pointer to a child folder of the current folder
 * 
 */
void push_folder(struct dir_entry_info * child_folder_ptr);

/**
 * @name pop_folder
 * @brief Pops folder off the folder stack
 * 
 * The current folder is now set to the previous folder from the popped folder.
 * 
 */
void pop_folder();

/**
 * @name print_dir_entry_info
 * @brief Prints the file/directory's detailed information
 * 
 * Displays the filename, extension, logical file size, and starting cluster of the specified file/directory.
 *
 * @param   folder_ptr     pointer to the file entry
 */
void print_dir_entry_info(struct dir_entry_info * entry_ptr);

/**
 * @name list_dir_entry_report
 * @brief Prints all content's detailed information of the current directory
 * 
 * Displays the filename, extension, logical file size, and starting cluster
 * of all the files/directories contained in the current directory
 * 
 */
void list_dir_entry_report();

/**
 * @name list_dir_entry_short
 * @brief Displays all contents in the current directory
 * 
 * Displays all the filenames and/or directories in the current folder.
 * 
 */
void list_dir_entry_short();

/**
 * @name print_curr_path
 * @brief Displays the current directory's path
 * 
 */
void print_curr_path();

/**
 * @name rename_entry
 * @brief Renames indicated file/folder's name
 * 
 * Ensures that it is a unique file name by 
 * checking all of the file names in the current file before renaming it.
 *
 * @param   parent_dir_entry    A pointer to the current directory
 * @param   file_entry          A pointer to the file entry to rename
 * @param   new_name            The file entry's new name
 * 
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t rename_entry(struct dir_entry_info * parent_dir_entry, struct dir_entry_info * file_entry, char * new_name);

/**
 * @name get_entry_by_name
 * @brief Retrieves the file/directory by name
 *
 * @param   parent_dir_entry    A pointer to the current directory of the file
 * @param   nameStr             The name of the file/directory to retrieve
 * 
 * @return Returns a pointer to the correct dir_entry_info struct or NULL if it is not found
 */
struct dir_entry_info * get_entry_by_name(const struct dir_entry_info * parent_dir_entry, char * nameStr);

/**
 * @name get_entry
 * @brief Retrieves the file/directory by it's full file path
 * 
 * @param   full_path    The full file path of the 
 */
struct dir_entry_info * get_entry(char * full_path);

/**
 * @name change_dir
 * @brief Change the current directory to the specified path
 *
 * @param  full_path    The path to the new location
 */
void change_dir(char * full_path);

/**
 * @name list_files_entry_ext
 * @brief Lists all the files with the indicated extension
 * 
 * Used when the wildcard '*' is indicated as the filename. 
 * (Ex: *.txt will list all the files with the 'txt' extension)
 *
 * @param   ext   extension of entry
 */
void list_files_entry_ext(const char * ext);

/**
 * @name list_files_entry_name
 * @brief Lists all the files with the indicated name
 * 
 * Used when the wildcard '*' is indicated as the extension.
 * (Ex: filename.* will list all the files with the name 'filename')
 * 
 * @param  name     Name of the file
 */
void list_files_entry_name(const char * name);

/**
 * @name list_file_report
 * @brief Lists the indicated file's full details
 * 
 * Displays the filename, extension, logical file size, and starting cluster of
 * the indicated name and extension. If wildcard is true (1) then it will list
 * the full details of the indicated.
 * (Example. *.txt will list the full report of all files with the 'txt' extension)
 *
 * @param   name        name of file
 * @param   ext         extension of file
 * @param   wildcard    1 or 0 (true or false)  
 * 
 */
void list_file_report(const char *name, const char *ext, int wildcard);

/**
 * @name print_report_heading
 * @brief Prints the heading of the report.
 */
void print_report_heading();