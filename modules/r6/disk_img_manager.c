#include "disk_img_manager.h"
#include "ansi.h"

char * current_image_file_path = NULL;

struct img_boot_sector * boot_sec = NULL;

static uint8_t * fat_arr = NULL;

struct dir_entry_info * root_dir_file_arr = NULL;

struct data_sector * data_area = NULL;

struct dir_entry_info * root_dir_entry = NULL;

/*
static void print_fat()
{
	printf("\n");
	int i = 0;
	uint16_t fat_val = 0;
	for (i = 0; i < 20; i++)
	{
		fat(&fat_val, i);
		//fat_val = *get_fat_val(0, i);
		printf("%#x  ", fat_val);
	}
	printf("\n");
}
*/

error_t load_image_file(const char * path_to_file)
{
    FILE * img_file = fopen(path_to_file, "rb");
    
    if(!img_file)
    {
        return E_FILE_NF;
    }
    
    current_image_file_path = malloc(strlen(path_to_file));
    memcpy(current_image_file_path, path_to_file, strlen(path_to_file));
    
    boot_sec = malloc(sizeof(struct img_boot_sector));
    fread(boot_sec, 512, 1, img_file);

	//printf("\n*debug* beofore fat @: %#x\n", ftell(img_file));
    int fats_bytes_size = boot_sec->byte_per_sector * boot_sec->sec_per_fat_num * boot_sec->fat_copies_num;
    fat_arr = malloc(fats_bytes_size);
    fread(fat_arr, fats_bytes_size, 1, img_file);
	//printf("\n*debug* after fat @: %#x\n", ftell(img_file));
    
    //int root_dir_byte_size = boot_sec->root_dir_max_num * sizeof(struct dir_entry_info);
    int root_dir_byte_size = boot_sec->root_dir_max_num * 32;
    root_dir_file_arr = malloc(root_dir_byte_size);
    fread(root_dir_file_arr, root_dir_byte_size, 1, img_file);
    
	//printf("\n*debug* beofore data_area @: %#x\n", ftell(img_file));

    //int data_area_byte_size = ((boot_sec->sec_num - 1 - (boot_sec->sec_per_fat_num * boot_sec->fat_copies_num)) * boot_sec->byte_per_sector) - (boot_sec->root_dir_max_num * sizeof(struct dir_entry_info));
    int data_area_byte_size = (boot_sec->sec_num * boot_sec->byte_per_sector) - ftell(img_file);
    data_area = malloc(data_area_byte_size);
    fread(data_area, data_area_byte_size, 1, img_file);
	//printf("\n*debug* data_area @: %#x\n", data_area);
	//printf("\n*debug* data_area size: %#x\n", data_area_byte_size);
	//printf("\n*debug* after data_area @: %#x\n", ftell(img_file));

    fclose(img_file);
    
    root_dir_entry = malloc(sizeof(struct dir_entry_info));
    memcpy(root_dir_entry->file_name, boot_sec->vol_label, 8);
    root_dir_entry->attributes = ATTRIBUTE_SUBD;
    root_dir_entry->first_log_clu = 0;
    root_dir_entry->file_size = 0;

	//print_fat();
    return E_NOERROR;
}

error_t write_image_file()
{
    if(!current_image_file_path)
    {
        return E_NULL_PTR;
    }
    printf("Info: Writing to \"%s\"...\n",current_image_file_path);
    FILE * img_file = fopen(current_image_file_path, "wb");
    
    if(!img_file)
    {
        return E_FILE_NF;
    }
    fseek(img_file , 0 , SEEK_SET);
    
    fwrite(boot_sec, 512, 1, img_file);
    
    int fats_bytes_size = boot_sec->byte_per_sector * boot_sec->sec_per_fat_num * boot_sec->fat_copies_num;
    fwrite(fat_arr, fats_bytes_size, 1, img_file);
    
    int root_dir_byte_size = boot_sec->root_dir_max_num * 32;
    fwrite(root_dir_file_arr, root_dir_byte_size, 1, img_file);
    
    int data_area_byte_size = (boot_sec->sec_num * boot_sec->byte_per_sector) - ftell(img_file);
    fwrite(data_area, data_area_byte_size, 1, img_file);
    
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
        free(root_dir_file_arr);
    }
    if(data_area)
        free(data_area);
    if(root_dir_entry)
        free(root_dir_entry);
    if(current_image_file_path)
        free(current_image_file_path);
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
    printf("%s%sFree space: %lu Bytes.%s%s\n\n", T_BOLD, T_RED, calc_free_space(), T_NRM, T_BOLD_OFF);
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

void str_to_ch_arr(char * dest, const char * src, const unsigned int size)
{
    unsigned int i = 0;
    unsigned char after_end = 0;
    for(i = 0; i < size; i++)
    {
        if(!after_end && src[i] == '\0')
            after_end = 1;
        
        dest[i] = after_end ? ' ' : src[i];
    }
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
    const uint16_t n = cluster_index;
	uint8_t first_val = *(get_fat_val(0, (3 * n) / 2));
	uint8_t second_val = *(get_fat_val(0, (1 + ((3 * n) / 2))));
	uint16_t tempVal = (second_val << 8) | first_val;
    if(n % 2)
    {//if it is odd
        *fat_val = (tempVal >> 4) & 0x0FFF;
    }
    else
    {//if it is even
        *fat_val = tempVal & 0x0FFF;
    }
}

void * get_data_ptr(const uint16_t data_area_sec_index)
{
    if(data_area_sec_index < 2)
        return NULL;
    
    return (void *)&data_area[data_area_sec_index - 2];
}



void write_fat(const uint16_t fat_val, const uint16_t cluster_index)
{
    if(fat_val & 0xF000)
        return;
        
    const uint16_t n = cluster_index;
	uint8_t * upper_val_0 = (get_fat_val(0, (3 * n) / 2));
	uint8_t * lower_val_0 = (get_fat_val(0, (1 + ((3 * n) / 2))));
	uint8_t * upper_val_1 = (get_fat_val(1, (3 * n) / 2));
	uint8_t * lower_val_1 = (get_fat_val(1, (1 + ((3 * n) / 2))));
    
    uint8_t temp_upper = 0;
    uint8_t temp_lower = 0;
    
    if(n % 2)
    {//if it is odd
        temp_upper = (uint8_t)((fat_val << 4) & 0x00F0) | (*upper_val_0 & 0x000F);
        temp_lower = (uint8_t)((fat_val >> 4) & 0x00FF);
    }
    else
    {//if it is even
        temp_upper = (uint8_t)(fat_val & 0x00FF);
        temp_lower = (uint8_t)((fat_val >> 8) & 0x000F) | (*lower_val_0 & 0x00F0);
    }
    
    *upper_val_0 = *upper_val_1 = temp_upper;
    *lower_val_0 = *lower_val_1 = temp_lower;
}

uint16_t find_unused_fat()
{
    
    unsigned int fat_num = (unsigned int)(boot_sec->sec_per_fat_num * boot_sec->byte_per_sector * ((float)2/3));
    uint16_t fat_val = 0x0FFF;
    
    unsigned int i = 1;
    do
    {
        i++;
        fat(&fat_val, i);
    }
    while(fat_val != 0x0000 && i < fat_num);
    
    //fat(&fat_val, i);
    //printf("*DEBUG* %#x out of %#x, val = %#x, Result %d\n", i, fat_num, fat_val, (fat_val != 0x0000 && i < fat_num));
    return i;
}

uint64_t calc_free_space()
{
    unsigned int i = 0;
    unsigned int fat_num = (unsigned int)(boot_sec->sec_per_fat_num * boot_sec->byte_per_sector * ((float)2/3));
    uint16_t free_sec_num = 0;
    uint16_t fat_val = 0x0FFF;
    for(i = 2; i < fat_num; i++)
    {
        fat(&fat_val, i);
        if(fat_val == 0x0000)
            free_sec_num++;
    }
    
    return ((uint64_t)free_sec_num * boot_sec->byte_per_sector);
}

void str_to_upper_case(char * str, const unsigned int len)
{
    int i = 0;
    for(i = 0; i < len; i++)
    {
        if('a' <= str[i] && str[i] <= 'z')
            str[i] -= ('a' - 'A');
    }
}

error_t seperate_file_name(const char * full_name, char * file_name, char * file_ext)
{
    int len = strlen(full_name);
    int temp = len - 1;
    for(; temp >= 0 && full_name[temp] != '.'; temp--);
    temp++;
    
    if((temp > 0 && (len - temp > 3 || temp - 1 > 8)) || (temp == 0 && (len > 8)))
        return E_INVSTRF; //ERROR Type 1
    
    //Reset the filename to empty first.
    memset(file_name, ' ', 8);
    memset(file_ext, ' ', 3);
    if(temp > 0)
    {
        memcpy(file_name, full_name, temp - 1);
        memcpy(file_ext, &full_name[temp], len - temp);
    }
    else
    {
        memcpy(file_name, full_name, len);
    }
    
    return E_NOERROR;
    
}

void get_full_name(char * dest, const struct dir_entry_info * entry)
{
    if(!dest || !entry)
        return;
    
    memset(dest, 0, 12);
    char file_name[9] = { 0 };
    char file_ext[4] = { 0 };
    ch_arr_to_str(file_name, entry->file_name, 8);
    ch_arr_to_str(file_ext, entry->extension, 3);
    if(strlen(file_ext) > 0)
        sprintf(dest, "%s.%s", file_name, file_ext);
    else
        sprintf(dest, "%s", file_name);
}

error_t validate_filename(const char * name_str)
{
    error_t result = E_NOERROR;
    while(*name_str && result == E_NOERROR)
    {
        switch(*name_str)
        {
            case '*':
            case '/':
            case '\\':
            case '?':
            case ':':
            case '"':
            case '<':
            case '>':
            case '|':
                result = E_NAMEINV;
                break;
        }
        name_str++;
    }
    return result;
}

void get_fat_date(struct fat_date * out_date, const uint16_t fat_date_value)
{
    /*
    out_date->year = (fat_date_value & 0x7F) + 1980;
    out_date->mon = (fat_date_value >> 7) & 0xF;
    out_date->day = (fat_date_value >> 11) & 0x1F;
    */
    out_date->day = fat_date_value & 0x001F;
    out_date->mon = (fat_date_value >> 5) & 0x000F;
    out_date->year = ((fat_date_value >> 9) & 0x007F) + 1980;
}


void get_fat_date_str(char * out_str, const uint16_t fat_date_value)
{
    struct fat_date date_result;
    get_fat_date(&date_result, fat_date_value);
    //sprintf(out_str, "%#X", fat_date_value);
    sprintf(out_str, "%2u/%2u/%2u", date_result.mon, date_result.day, date_result.year % 100);
}


void get_fat_time(struct fat_time * out_time, const uint16_t fat_time_value)
{
    out_time->se = (fat_time_value & 0x001F) * 2;
    out_time->mi = (fat_time_value >> 5) & 0x003F;
    out_time->hr = ((fat_time_value >> 11) & 0x001F);
}


void get_fat_time_str(char * out_str, const uint16_t fat_time_value)
{
    struct fat_time time_result;
    get_fat_time(&time_result, fat_time_value);
    //sprintf(out_str, "%#X", fat_time_value);
    sprintf(out_str, "%2u:%2u:%2u", time_result.hr, time_result.mi, time_result.se);
}