/**
 * @file disk_file_manager.h
 * @author Thunder Krakens
 * @date April 28th, 2016
 * @brief Disk File Manager
 * 
 * This contains functions that deal with files
 * 
 * @version R6
 *
 */


/**
 * @brief Directory entry struct in sector
 * @ref dir_entry_info
 * @see DirEntryStruct
 */
struct dir_entry_info;

/**
 * @name type_file
 * @brief Prints the contents of a file 
 * 
 * This function will print any contents of a file with the extensions "TXT", "BAT", "C", or "HTM" in pagination form. 
 *
 * @param  file_entry_ptr   The pointer to the file entry
 * @return The appropiate error code. See @ref errno.h for details.
 */
error_t type_file(struct dir_entry_info * file_entry_ptr);

/**
 * @name extract_file
 * @brief Extracts a file's data contents into new file outside of the disk image.
 *
 * @param   file_entry_ptr  The pointer to the file entry
 * @param   out_file_path   The destination to save the new 'copied' file 
 * 
 * @return The appropiate error code. See @ref errno.h for details.
 */
error_t extract_file(struct dir_entry_info * file_entry_ptr, const char * out_file_path);

/**
 * @name import_file
 * @brief Imports a file into a directory
 *
 * @param   in_file_path    Where the file to be imported is located
 * @param   dest_dir        The directory destination for the imported file
 * @return The appropiate error code. See @ref errno.h for details.
 */
error_t import_file(const char * in_file_path, struct dir_entry_info * dest_dir);

/**
 * @name move_file
 * @brief Moves files from destination to another
 * 
 * @param   file_entry      The file entry pointer to be moved
 * @param   dest_dir        The destination of the moved file
 * @return The appropiate error code. See @ref errno.h for details.
 */
error_t move_file(struct dir_entry_info * file_entry, struct dir_entry_info * dest_dir);

error_t delete_file(struct dir_entry_info * file_entry);