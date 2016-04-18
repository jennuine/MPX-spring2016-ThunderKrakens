#include "disk_img_manager.h"


struct img_boot_sector * boot_sec = NULL;

static uint8_t * fat_arr = NULL;

struct dir_entry_info * root_dir_file_arr = NULL;

struct data_sector * data_area = NULL;

error_t load_image_file(const char * path_to_file)
{
    FILE * img_file = fopen(path_to_file, "rb");
    
    if(!img_file)
    {
        return E_FILE_NF;
    }
    
    boot_sec = malloc(sizeof(struct img_boot_sector));
    fread(boot_sec, 512, 1, img_file);
    
    int fats_bytes_size = boot_sec->byte_per_sector * boot_sec->sec_per_fat_num * boot_sec->fat_copies_num;
    fat_arr = malloc(fats_bytes_size);
    fread(fat_arr, fats_bytes_size, 1, img_file);
    
    int root_dir_byte_size = boot_sec->root_dir_max_num * sizeof(struct dir_entry_info);
    root_dir_file_arr = malloc(root_dir_byte_size);
    fread(root_dir_file_arr, root_dir_byte_size, 1, img_file);
    
    int data_area_byte_size = ((boot_sec->sec_num - 1 - (boot_sec->sec_per_fat_num * boot_sec->fat_copies_num)) * boot_sec->byte_per_sector) - (boot_sec->root_dir_max_num * sizeof(struct dir_entry_info));
    data_area = malloc(data_area_byte_size);
    fread(data_area, data_area_byte_size, 1, img_file);
    
    fclose(img_file);
    return E_NOERROR;
}


void clean_buffers()
{
    if(boot_sec)
        free(boot_sec);
        
    if(fat_arr)
        free(fat_arr);
        
    if(root_dir_file_arr)
    {   
        //*root_dir_file_arr--;
        free(root_dir_file_arr);
    }
    if(data_area)
        free(data_area);
}

void print_boot_sec_info(const struct img_boot_sector * boot_sec)
{
    if(!boot_sec)
        return;
        
    char vol_label[12];
    char file_sys_type[9];
    ch_arr_to_str(vol_label, boot_sec->vol_label, 11);
    ch_arr_to_str(file_sys_type, boot_sec->file_sys_type, 8);
    
    printf("Bytes per Sector:              %d\n", boot_sec->byte_per_sector);
    printf("Sectors per Cluster:           %d\n", boot_sec->sector_per_cluster);
    printf("Num of Reserved Sectors:       %d\n", boot_sec->reserved_sec_num);
    printf("Num of FAT Copies:             %d\n", boot_sec->fat_copies_num);
    printf("Max Num of Root Dir Entries:   %d\n", boot_sec->root_dir_max_num);
    printf("Total num of Sec in File Sys:  %d\n", boot_sec->sec_num);
    printf("Num of Sectors per FAT:        %d\n", boot_sec->sec_per_fat_num);
    printf("Sectors per Track:             %d\n", boot_sec->sec_per_track);
    printf("Number of heads:               %d\n", boot_sec->head_num);
    printf("Total sector count for FAT32 : %d\n", boot_sec->total_sec_fat32);
    printf("Boot Signature:                %d\n", boot_sec->boot_sign);
    printf("Volume ID:                     %d\n", boot_sec->vol_id);
    printf("Volume Label:                  %s\n", vol_label);
    printf("File System Type:              %s\n", file_sys_type);
    printf("\n");
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



/**
 * get the specific meanningful 12-bit value from the FAT array.
 * As said in page 4 of FAT12 File System Format Information. Convert 2*1 byte to 12 bit.
**/
void fat(uint16_t * fat_val, const uint16_t cluster_index)
{
    const uint16_t n = cluster_index + 2;
    uint16_t tempVal = 0x0000;
    if(n % 2)
    {//if it is odd
        uint8_t higher_val = *get_fat_val(1, (3 * n) / 2);
        uint8_t lower_val  = *get_fat_val(1, (1 + ((3 * n) / 2)));
        
        tempVal  = ((uint16_t)higher_val & 0x0010);
        tempVal  = tempVal | ((uint16_t)lower_val << 4);
        *fat_val = tempVal;
    }
    else
    {//if it is even
        uint8_t higher_val = *get_fat_val(1, (3 * n) / 2);
        uint8_t lower_val  = *get_fat_val(1, (1 + ((3 * n) / 2)));
        
        tempVal  = (uint16_t)higher_val; //high 8 bits
        tempVal  = tempVal | (((uint16_t)lower_val & 0x0001) << 8);//low 4 bits
        *fat_val = tempVal;
    }
}
