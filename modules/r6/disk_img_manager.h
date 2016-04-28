/**
 * @file disk_img_manager.h
 * @author Thunder Krakens
 * @date April 28th, 2016
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

/** 
 * @brief Structure containing the Boot Sector's information and data
 */
PACKED(
    struct img_boot_sector
    {
        uint8_t     ignore1[11]; /**< Ignore file data. Number of bytes 11 and starting at byte location 0.*/
        uint16_t    byte_per_sector; /**< Bytes per sector. Number of bytes 2 and starting at byte location 11.*/
        uint8_t     sector_per_cluster; /**< Sectors per cluster. Number of bytes 1 and starting at byte location 13.*/
        uint16_t    reserved_sec_num; /**< Number of reserved rectors. Number of bytes 2 and starting at byte location 14.*/
        uint8_t     fat_copies_num; /**< Number of FAT copies. Number of bytes 1 and starting at byte location 16.*/
        uint16_t    root_dir_max_num; /**< Max number of root directory entries. Number of bytes 2 and starting at byte location 17.*/
        uint16_t    sec_num; /**< Total number of sectors in the File System. Number of bytes 2 and starting at byte location 19.*/
        uint8_t     ignore2; /**< Ignore file data. Number of bytes 1 and starting at byte location 21.*/
        uint16_t    sec_per_fat_num; /**< Number of Sectors per FAT. Number of bytes 2 and starting at byte location 22.*/
        uint16_t    sec_per_track; /**< Sectors per track. Number of bytes 2 and starting at byte location 24.*/
        uint16_t    head_num; /**< Number of heads. Number of bytes 2 and starting at byte location 26.*/
        uint32_t    ignore3; /**< Ignore file data. Number of bytes 4 and starting at byte location 28.*/
        uint32_t    total_sec_fat32; /**< Total sector count for FAT32. Number of bytes 4 and starting at byte location 32.*/
        uint16_t    ignore4; /**< Ignore file data. Number of bytes 2 and starting at byte location 36.*/
        uint8_t     boot_sign; /**< Boot signature. Number of bytes 1 and starting at byte location 38.*/
        uint32_t    vol_id; /**< Volume ID. Number of bytes 4 and starting at byte location 39.*/
        uint8_t     vol_label[11]; /**< Volume label (or name of File System) as an ASCII string. Number of bytes 11 and starting at byte location 43.*/
        uint8_t     file_sys_type[8]; /**< File System Type as an ASCII string. Number of bytes 8 and starting at byte location 54.*/
        uint8_t     ignore5[450]; /**< Ignore file data. Number of bytes 450 and starting at byte location 62.*/
    }
);

/** 
 * @brief Structure containing the directory/file entry's information and data in sector
 */
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
/** 
 * @brief Structure containing the sector's data
 */
    struct data_sector
    {
        uint8_t     data[512];  /**< The sector's raw data. Number of bytes 512. */
    }
);

/** 
* @brief Structure containing the date information
*/
PACKED(
    struct fat_date
    {
        uint8_t  day; /**< Day. */
        uint8_t  mon; /**< Month. */
        uint16_t year; /**< Year. */
    }
);

/** 
* @brief Structure containing the time information
*/
PACKED(
    struct fat_time
    {
        uint8_t se; /**< Seconds. */
        uint8_t mi; /**< Minutes. */
        uint8_t hr; /**< Hours. */
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
 * @brief Loads the indeicated image file
 *
 * @param   path_to_file     String path to the image file
 *
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t load_image_file(const char * path_to_file);

/**
 * @name write_image_file
 * @brief Writes to image file
 * 
 * Will write all changes made to the image
 * 
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t write_image_file();

/**
 * @name print_boot_sec_info
 * @brief Displays the boot sector information
 *
 * @param   boot_sec     Pointer to the boot sector
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
 * @brief Converts character array to a string
 *
 * @param   dest    Variable to hold converted string
 * @param   src     Source char array
 * @param   size    size of char array
 * 
 */
void ch_arr_to_str(char * dest, const char * src, const unsigned int size);

/**
 * @name str_to_ch_arr
 * @brief Convert string to character array
 *
 * @param   dest    Variable to hold converted char array
 * @param   src     Source string
 * @param   size    size of string
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
 * @brief Retrieve specified 12-bit value from the FAT array.
 * 
 * Converts 2*1 byte to 12 bit.
 * 
 * @param   fat_val         FAT Value
 * @param   cluster_index   Cluster Index
 * 
 **/
void fat(uint16_t * fat_val, const uint16_t cluster_index);

/**
 * @name get_data_ptr
 * @brief Retrieves the memory address of the data
 *
 * @param   data_area_sec_index     Data sector index
 * 
 */
void * get_data_ptr(const uint16_t data_area_sec_index);

/**
 * @name write_fat
 * @brief Writes the FAT value at the cluster index
 *
 * @param   fat_val         FAT value
 * @param   cluster_index   Cluster index
 * 
 */
void write_fat(const uint16_t fat_val, const uint16_t cluster_index);

/**
 * @name find_unused_fat
 * @brief Retrieves unused FAT index
 * 
 * @return the index of the FAT value for the fat array
 */
uint16_t find_unused_fat();

/**
 * @name calc_free_space
 * @brief Calculates the free space in FAT
 * 
 * @return the value of free space.
 */
uint64_t calc_free_space();

/**
 * @name str_to_upper_case 
 * @brief Converts string characters to uppercase
 * 
 * @param   str     String to conver to uppercase
 * @param   len     The length of the string
 */
void str_to_upper_case(char * str, const unsigned int len);

/**
 * @name separate_file_name
 * @brief Separates the filename string by name and extension.
 * 
 * @param   full_name   The full name of the file (Example: file.ext)
 * @param   file_name   The variable to store the filename
 * @param   file_ext    The variable to store the file's extension
 * 
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t seperate_file_name(const char * full_name, char * file_name, char * file_ext);

/**
 * @name get_fat_date
 * @brief Stores FAT's date
 * 
 * @param   out_date        A pointer to the struct that will hold the date
 * @param   fat_date_value  The FAT date value    
 */
void get_fat_date(struct fat_date * out_date, const uint16_t fat_date_value);

/**
 * @name get_fat_date_str
 * @brief Stores the FAT's date in a string
 * 
 * @param   out_str         The string to store the date's information
 * @param   fat_date_value  The FAT date value
 */
void get_fat_date_str(char * out_str, const uint16_t fat_date_value);

/**
 * @name set_fat_time
 * @brief Sets the FAT time 
 * 
 * @param   in_time             A pointer the time structure containing the information 
 * @param   out_fat_time_value  The FAT time value
 */
void set_fat_time(const struct fat_time * in_time, uint16_t * out_fat_time_value);

/**
 * @name get_fat_time
 * @brief Stores the FAT time
 * 
 * @param   out_time         A pointer to the structure containing the time
 * @param   fat_date_value   The FAT time value
 */
void get_fat_time(struct fat_time * out_time, const uint16_t fat_time_value);

/**
 * @name get_fat_time_str
 * @brief Stores the FAT's time in a string
 * 
 * @param   out_str         The string to store the time's information
 * @param   fat_date_value  The FAT date value
 */
void get_fat_time_str(char * out_str, const uint16_t fat_time_value);
