/**
 * @file disk_file_manager.h
 * @author Thunder Krakens
 * @date April 27th, 2016
 * @brief Disk File Manager
 * @version R6
 *
 */

//#include "disk_img_manager.h"

struct dir_entry_info;

/**
 * @name type_file
 * @brief The type of the file
 *
 * @param  file_entry_ptr   The entry pointer in a file
 * 
 */

void type_file(struct dir_entry_info * file_entry_ptr);

/**
 * @name extract_file
 * @brief Extracts a file
 *
 * @param   file_entry_ptr  The entry pointer in a file
 * @param   out_file_path   The destination of the extracted file
 * 
 */

error_t extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path);

/**
 * @name import_file
 * @brief Imports a file
 *
 * @param   in_file_path    Where the file to be imported
 * @param   dest_dir        The destination of imported file
 * 
 */

error_t import_file(const char * in_file_path, struct dir_entry_info * dest_dir);

/**
 * @name move_file
 * @brief Moves files from destination to another
 *
 * @param   file_entry      The entry that needs to be moved
 * @param   dest_dir        The destination of the moved file
 * 
 */

error_t move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir);