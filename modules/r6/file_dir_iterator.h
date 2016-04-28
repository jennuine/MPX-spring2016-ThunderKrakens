/**
 * @file file_dir_iterator.h
 * @author Thunder Krakens
 * @date April 28th, 2016
 * @brief File Directory Iterator
 * @version R6
 *
 */
#include <stdint.h>

#define ROOT_DIR_SEC_INDEX 0

/** @struct file_iter */
struct file_itr;

/** @struct dir_itr */
struct dir_itr;

/** @struct img_writer */
struct img_writer;

/**
 * @name init_file_itr
 * @brief Initializes the file iterator at the specified sector index
 * 
 * @return A pointer to the initialized @ref file_itr
 */
struct file_itr * init_file_itr(const uint16_t sec_index);

/**
 * @name fitr_begin
 * @brief Starts the iterator
 *
 * @param   itr_ptr     Pointer of the iterator
 */
void fitr_begin(struct file_itr * itr_ptr);

/**
 * @name fitr_end
 * @brief Checks if the iterator has reached the end of FAT (the last cluster in the file)
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 * @return  0 or 1 (true or false)
 */
uint8_t fitr_end(struct file_itr * itr_ptr);

/**
 * @name fitr_next
 * @brief Jumps to the next section in FAT
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 */
void fitr_next(struct file_itr * itr_ptr);

/**
 * @name fitr_get
 * @brief Retrieves the data at the iterator 
 *
 * @param   itr_ptr     Pointer of the iterator
 */

struct data_sector * fitr_get(struct file_itr * itr_ptr);

/**
 * @name init_dir_itr
 * @brief Initializes the directory iterator at the specified sector's index
 *
 * @param   sec_index     Sector index
 */
struct dir_itr * init_dir_itr(const uint16_t sec_index);

/**
 * @name ditr_set_filter
 * @brief Set's the directory iterator's attribute filter
 *
 * @param   itr_ptr         Pointer of the iterator
 * @param   attr_filter     Attribute filter
 */
void ditr_set_filter(struct dir_itr * itr_ptr, uint8_t attr_filter);

/**
 * @name ditr_begin
 * @brief Starts the directory iterator
 *
 * @param   itr_ptr     Pointer of the iterator
 */
void ditr_begin(struct dir_itr * itr_ptr);

/**
 * @name ditr_set_find_unused
 * @brief Set up a search in directory iterator
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 */

void ditr_set_find_unused(struct dir_itr * itr_ptr);

/**
 * @name ditr_end
 * @brief Ends the directory iterator
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 */

uint8_t ditr_end(struct dir_itr * itr_ptr);

/**
 * @name ditr_next
 * @brief Finds the next section in directory
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 */

void ditr_next(struct dir_itr * itr_ptr);

/**
 * @name ditr_get
 * @brief Retrieve information for the directory iterator
 *
 * @param   itr_ptr     Pointer of the iterator
 * 
 */

struct dir_entry_info * ditr_get(struct dir_itr * itr_ptr);

/**
 * @name init_img_writer
 * @brief Initialize the image writer
 *
 * @param   entry_ptr     Entry pointer
 * 
 */

struct img_writer * init_img_writer(struct dir_entry_info * entry_ptr);

/**
 * @name iw_write
 * @brief Function to write the image
 *
 * @param   writer_ptr    Pointer of the writer
 * @param   data          Construct the data sector
 * 
 */

void iw_write(struct img_writer * writer_ptr, const struct data_sector * data);