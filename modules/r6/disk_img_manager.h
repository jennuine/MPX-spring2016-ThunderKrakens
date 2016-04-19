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
    uint8_t     file_name[8];
    uint8_t     extension[3];
    uint8_t     attributes;
    uint16_t    reserved;
    uint16_t    create_time;
    uint16_t    create_date;
    uint16_t    last_acc_date;
    uint16_t    ignore1;
    uint16_t    last_wri_time;
    uint16_t    last_wri_date;
    uint16_t    first_log_clu;
    uint32_t    file_size;
}
);


PACKED(
struct data_sector
{
    uint8_t     data[512];
}
);

#define ATTRIBUTE_READ 0x01
#define ATTRIBUTE_HIDD 0x02
#define ATTRIBUTE_SYST 0x04
#define ATTRIBUTE_VOLL 0x08

#define ATTRIBUTE_SUBD 0x10
#define ATTRIBUTE_ARCH 0x20
#define ATTRIBUTE_UUS1 0x40
#define ATTRIBUTE_UUS2 0x80

extern struct img_boot_sector * boot_sec;

extern struct dir_entry_info * root_dir_file_arr;

extern struct data_sector * data_area;


error_t load_image_file(const char * path_to_file);

void print_boot_sec_info(const struct img_boot_sector * boot_sec);

void clean_buffers();

void ch_arr_to_str(char * dest, const char * src, const unsigned int size);

uint8_t * get_fat_val(const unsigned int copy_index, const unsigned int byte_index);


/**
 * get the specific meanningful 12-bit value from the FAT array.
 * As said in page 4 of FAT12 File System Format Information. Convert 2*1 byte to 12 bit.
**/
void fat(uint16_t * fat_val, const uint16_t cluster_index);

void * get_data_ptr(const uint16_t data_area_sec_index);