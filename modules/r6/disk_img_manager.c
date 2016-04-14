#include "disk_img_manager.h"

struct img_boot_sector * boot_sec = NULL;

static uint8_t * fat_arr = NULL;

struct dir_entry_info * root_dir_file_arr = NULL;

struct data_sector * data_area = NULL;

void load_image_file(const char * path_to_file)
{
    FILE * img_file = fopen(path_to_file, "rb");
    if(!img_file)
    {
        printf("Failed to read the image file!\n");
        return;
    }
    
    boot_sec = malloc(sizeof(struct img_boot_sector));
    fread(boot_sec, 512, 1, img_file);
    
    print_boot_sec_info(boot_sec);
    
    int fats_bytes_size = boot_sec->byte_per_sector * boot_sec->sec_per_fat_num * boot_sec->fat_copies_num;
    fat_arr = malloc(fats_bytes_size);
    fread(fat_arr, fats_bytes_size, 1, img_file);
    
    int root_dir_byte_size = boot_sec->root_dir_max_num * sizeof(struct dir_entry_info);
    root_dir_file_arr = malloc(root_dir_byte_size);
    fread(root_dir_file_arr, root_dir_byte_size, 1, img_file);
    
    int data_area_byte_size = ((boot_sec->sec_num - 1 - (boot_sec->sec_per_fat_num * boot_sec->fat_copies_num)) * boot_sec->byte_per_sector) - (boot_sec->root_dir_max_num * sizeof(struct dir_entry_info));
    data_area = malloc(data_area_byte_size);
    fread(data_area, data_area_byte_size, 1, img_file);
    
    if(img_file)
        fclose(img_file);
    
}

void clean_buffers()
{
    if(boot_sec)
        free(boot_sec);
        
    if(fat_arr)
        free(fat_arr);
        
    if(root_dir_file_arr)
        free(root_dir_file_arr);
        
    if(data_area)
        free(data_area);
}

void print_boot_sec_info(const struct img_boot_sector * boot_sec)
{
    if(!boot_sec)
        return;
        
    printf("Bytes per Sector:              %d\n", boot_sec->byte_per_sector);
    printf("Sectors per Cluster:           %d\n", boot_sec->sector_per_cluster);
    printf("Num of Reserved Sectors:       %d\n", boot_sec->reserved_sec_num);
    printf("Num of FAT Copies:             %d\n", boot_sec->fat_copies_num);
    printf("Max Num of Root Dir Entries:   %d\n", boot_sec->root_dir_max_num);
    printf("Total num of Sec in File Sys:  %d\n", boot_sec->sec_num);
    printf("Num of Sectors per FAT:        %d\n", boot_sec->sec_per_fat_num);
    //we need more from here...
}

void ch_arr_to_str(char * dest, const char * src, const unsigned int size)
{
    unsigned int pos = 0;
    while(*src && !isspace(*src) && pos < size)
    {
        *dest = *src;
        dest++;
        src++;
        pos++;
    }
    *dest = '\0';
}

uint8_t * get_fat_val(const unsigned int copy_index, const unsigned int byte_index)
{
    return &fat_arr[(copy_index * boot_sec->byte_per_sector * boot_sec->sec_per_fat_num) + byte_index];
}


#if 0

/**
 * get the specific meanningful 12-bit value from the FAT array.
 * As said in page 4 of FAT12 File System Format Information. Convert 2*1 byte to 12 bit.
**/
uint16_t fat(const uint16_t cluster_index);

#endif