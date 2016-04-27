/**
 * @file disk_img_manager.h
 * @author Thunder Krakens
 * @date April 27th, 2016
 * @brief Disk Image Manager
 * @version R6
 *
 */



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../errno.h"

#include "../packing.h"

PACKED(
struct img_boot_sector
{
    uint8_t     ignore1[11];
    uint16_t    byte_per_sector;
    uint8_t     sector_per_cluster;
    uint16_t    reserved_sec_num;
    uint8_t     fat_copies_num;
    uint16_t    root_dir_max_num;
    uint16_t    sec_num;
    uint8_t     ignore2;
    uint16_t    sec_per_fat_num;
    uint16_t    sec_per_track;
    uint16_t    head_num;
    uint32_t    ignore3;
    uint32_t    total_sec_fat32;
    uint16_t    ignore4;
    uint8_t     boot_sign;
    uint32_t    vol_id;
    uint8_t     vol_label[11];
    uint8_t     file_sys_type[8];
    uint8_t     ignore5[450];
}
);


PACKED(
struct dir_entry_info
{
    uint8_t     file_name[8]; /**< File name in ASCII Characters. Number of bytes 8 and starting at byte location 0.*/
    uint8_t     extension[3];  /**< File extension in ASCII Characters. Number of bytes 3 and starting at byte location 8.*/
    uint8_t     attributes;  /**< File attributes. Number of bytes 1 and starting at byte location 11.*/
    uint16_t    reserved;  /**< Reserved. Number of bytes 2 and starting at byte location 12.*/
    uint16_t    create_time; /**< Time created file. Number of bytes 2 and starting at byte location 14.*/
    uint16_t    create_date; /**< Date created file. Number of bytes 2 and starting at byte location 16.*/
    uint16_t    last_acc_date; /**< Date last accessed file. Number of bytes 2 and starting at byte location 18.*/
    uint16_t    ignore1; /**< Ignore file data. Number of bytes 2 and starting at byte location 20.*/
    uint16_t    last_wri_time; /**< Last modified file time. Number of bytes 2 and starting at byte location 22.*/
    uint16_t    last_wri_date; /**< Last modified file date. Number of bytes 2 and starting at byte location 24.*/
    uint16_t    first_log_clu; /**< First logical cluster specifies where the file or subdirectory begins. Number of bytes 2 and starting at byte location 26.*/
    uint32_t    file_size;/**< File size in bytes. Number of bytes 4 and starting at byte location 28.*/
}
);


PACKED(
struct data_sector
{
    uint8_t     data[512];
}
);

PACKED(
struct fat_date
{
    uint8_t  day;
    uint8_t  mon;
    uint16_t year;
}
);

PACKED(
struct fat_time
{
    uint8_t se;
    uint8_t mi;
    uint8_t hr;
}
);

#define ATTRIBUTE_READ 0x01 // read-only
#define ATTRIBUTE_HIDD 0x02 // hidden
#define ATTRIBUTE_SYST 0x04 // system
#define ATTRIBUTE_VOLL 0x08 // volume label

#define ATTRIBUTE_SUBD 0x10 // subdirectory
#define ATTRIBUTE_ARCH 0x20 // archive
#define ATTRIBUTE_UUS1 0x40 // device 
#define ATTRIBUTE_UUS2 0x80 // unused

#define ATTR_ARCH_ONLY (!ATTRIBUTE_SYST & !ATTRIBUTE_VOLL & !ATTRIBUTE_SUBD & !ATTRIBUTE_UUS1 & !ATTRIBUTE_UUS2)
extern struct img_boot_sector * boot_sec;

extern struct dir_entry_info * root_dir_file_arr;

extern struct data_sector * data_area;

extern struct dir_entry_info * root_dir_entry;

/**
 * @name load_image_file
 * @brief Loads selected image file
 *
 * @param   path_to_file     path to the image file
 * 
 */

error_t load_image_file(const char * path_to_file);

/**
 * @name write_image_file
 * @brief Writes to image file
 * 
 */

error_t write_image_file();

/**
 * @name print_boot_sec_info
 * @brief Prints boot sector information to the user
 *
 * @param   boot_sec     Information of boot sector from the image
 * 
 */

void print_boot_sec_info(const struct img_boot_sector * boot_sec);

/**
 * @name clean_buffer
 * @brief Clears the buffer
 * 
 */

void clean_buffers();

/**
 * @name ch_arr_to_str
 * @brief Convert character array to string
 *
 * @param   dest    destination of output
 * @param   src     input of character array
 * @param   size    size of output
 * 
 */

void ch_arr_to_str(char * dest, const char * src, const unsigned int size);

/**
 * @name str_to_ch_arr
 * @brief Convert string to character array
 *
 * @param   dest    destination of output
 * @param   src     input of string
 * @param   size    size of output 
 * 
 */

void str_to_ch_arr(char * dest, const char * src, const unsigned int size);

/**
 * @name get_fat_val
 * @brief Retrieve FAT values
 *
 * @param   copy_index     Copies FAT Index
 * @param   byte_index     Retrieves index in bytes
 * 
 */

uint8_t * get_fat_val(const unsigned int copy_index, const unsigned int byte_index);


/**
 * @name fat
 * @brief get the specific meanningful 12-bit value from the FAT array. As said in page 4 of FAT12 File System Format Information.
 * Convert 2*1 byte to 12 bit.
 * 
 * @param   fat_val         FAT Values
 * @param   cluster_index   Cluster Index
 * 
**/

void fat(uint16_t * fat_val, const uint16_t cluster_index);

/**
 * @name get_data_ptr
 * @brief The pointer of data index
 *
 * @param   data_area_sec_index     Data sector index
 * 
 */

void * get_data_ptr(const uint16_t data_area_sec_index);

/**
 * @name write_fat
 * @brief write a FAT index
 *
 * @param   fat_val         FAT value
 * @param   cluster_index   Cluster index
 * 
 */

void write_fat(const uint16_t fat_val, const uint16_t cluster_index);

/**
 * @name find_unused_fat
 * @brief Search for unused FAT
 * 
 */

uint16_t find_unused_fat();

/**
 * @name calc_free_space
 * @brief Calculate available free space
 * 
 */

uint64_t calc_free_space();

/**
 * @name str_to_upper_case
 * @brief Convert string to upper case
 *
 * @param   str     Input string
 * @param   len     Length of string
 * 
 */

void str_to_upper_case(char * str, const unsigned int len);

/**
 * @name seperate_file_name
 * @brief Seperate file name from extension
 *
 * @param   full_name       Full name of file
 * @param   file_name       Output of file name
 * @param   file_ext        File extension
 * 
 */

error_t seperate_file_name(const char * full_name, char * file_name, char * file_ext);

/**
 * @name get_fat_date
 * @brief Get the date of FAT 
 *
 * @param   out_date            Output date
 * @param   fat_date_value      Retrieve FAT Date value
 * 
 */

void get_fat_date(struct fat_date * out_date, const uint16_t fat_date_value);

/**
 * @name get_fat_date_str
 * @brief Convert FAT date to str
 *
 * @param   out_date            Output date
 * @param   fat_date_value      Retrieve FAT Date value
 * 
 */

void get_fat_date_str(char * out_str, const uint16_t fat_date_value);

/**
 * @name set_fat_time
 * @brief Sets the time of FAT
 *
 * @param   in_time                 Input time
 * @param   out_fat_time_value      Output time
 * 
 */

void set_fat_time(const struct fat_time * in_time, uint16_t * out_fat_time_value);