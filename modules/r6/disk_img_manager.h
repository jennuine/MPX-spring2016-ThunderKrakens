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

error_t load_image_file(const char * path_to_file);

void print_boot_sec_info(const struct img_boot_sector * boot_sec);

void clean_buffers();

void ch_arr_to_str(char * dest, const char * src, const unsigned int size);

void str_to_ch_arr(char * dest, const char * src, const unsigned int size);

uint8_t * get_fat_val(const unsigned int copy_index, const unsigned int byte_index);


/**
 * get the specific meanningful 12-bit value from the FAT array.
 * As said in page 4 of FAT12 File System Format Information. Convert 2*1 byte to 12 bit.
**/
void fat(uint16_t * fat_val, const uint16_t cluster_index);

void * get_data_ptr(const uint16_t data_area_sec_index);

void write_fat(const uint16_t fat_val, const uint16_t cluster_index);

uint16_t find_unused_fat();

void str_to_upper_case(char * str, const unsigned int len);

error_t seperate_file_name(const char * full_name, char * file_name, char * file_ext);