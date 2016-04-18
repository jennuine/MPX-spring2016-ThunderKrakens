#include <stdint.h>

#define ROOT_DIR_SEC_INDEX 0

struct file_itr;

struct dir_itr;

struct file_itr * init_file_itr(const uint16_t sec_index);

void fitr_begin(struct file_itr * itr_ptr);

uint8_t fitr_end(struct file_itr * itr_ptr);

void fitr_next(struct file_itr * itr_ptr);

struct data_sector * fitr_get(struct file_itr * itr_ptr);



struct dir_itr * init_dir_itr(const uint16_t sec_index);

void ditr_set_filter(struct dir_itr * itr_ptr, uint8_t attr_filter);

void ditr_begin(struct dir_itr * itr_ptr);

uint8_t ditr_end(struct dir_itr * itr_ptr);

void ditr_next(struct dir_itr * itr_ptr);

struct dir_entry_info * ditr_get(struct dir_itr * itr_ptr);

uint16_t data_addr_to_sec_i(void * addr);