/**
 * @file mcb.c
 * @author Thunder Krakens
 * @date April 8th, 2016
 * @brief Memory Control Block
 * @version R5
 *
 */

#include "mcb.h"
#include <system.h>
#include <string.h>
#include <mem/heap.h>
#include "../mpx_supt.h"
#include "../r1/r1.h"
#include "../r2/pcb.h"


u32int start_of_memory;
u32int end_of_memory;

struct mcb * free_mem_list;
struct mcb * allocated_mem_list;

/**
 * @name get_mcb_total_size
 * @brief Finds MCB's total size
 *
 * @param   mem_size  The size of memory
 *
 * @return the total size of memory
 */
static u32int get_mcb_total_size(u32int mem_size)
{
    return ( sizeof(struct mcb) + sizeof(struct cmcb) + mem_size + sizeof(struct lmcb));
}

/**
 * @name init_mem_block
 * @brief Initiate memory block
 *
 * @param   start_pos  Starting position
 * @param   size        Size of MCB
 * @param   type        Type of MCB
 * @return VOID
 */
static void init_mem_block(struct mcb * start_pos, u32int size, enum mcb_type type)
{
    void * start_pos_ptr = (void *)start_pos;
    start_pos->complete_mcb = (struct cmcb *)(start_pos_ptr + sizeof(struct mcb));
    start_pos->limited_mcb = (struct lmcb *)(start_pos_ptr + sizeof(struct mcb) + sizeof(struct cmcb) + size);
    start_pos->complete_mcb->begin_address = (void *)(start_pos_ptr + sizeof(struct mcb) + sizeof(struct cmcb));
    
    start_pos->complete_mcb->size = start_pos->limited_mcb->size = size;
    start_pos->complete_mcb->type = start_pos->limited_mcb->type = type;
    
    start_pos->prev = start_pos->next = NULL;
}

/**
 * @name prev_adjacent_mcb
 * @brief Previous adjacent MCB in Memory
 *
 * @param  mcb_ptr  MCB Pointer
 *
 * @return the pointer to the previous MCB
 */
static struct mcb * prev_adjacent_mcb(struct mcb * mcb_ptr)
{    
    struct lmcb * prev_lmcb = (struct lmcb *)((u32int)mcb_ptr - sizeof(struct lmcb));
    
    //printf("*Debug* prev_adj: 1. %X \n", prev_lmcb);
    if((u32int)prev_lmcb <= start_of_memory)
        return NULL;
    
    struct mcb * prev_mcb = (struct mcb *)((u32int)mcb_ptr - get_mcb_total_size(prev_lmcb->size));
    
    //printf("*Debug* prev_adj: 3. %X \n", prev_mcb);
    return prev_mcb;
}

/**
 * @name next_adjacent_mcb
 * @brief Next adjacent MCB in Memory
 *
 * @param  mcb_ptr  MCB Pointer
 *
 * @return the pointer to the next MCB
 */
static struct mcb * next_adjacent_mcb(struct mcb * mcb_ptr)
{    
    struct mcb * next_mcb = (struct mcb *)((u32int)mcb_ptr + get_mcb_total_size(mcb_ptr->complete_mcb->size));
    
    if((u32int)next_mcb >= end_of_memory)
        return NULL;
    
    return next_mcb;
}

/**
 * @name find_mcb_by_address
 * @brief Finds the MCB by address
 *
 * @param  mem_ptr  Memory address pointer
 *
 * @return Pointer to the MCB
 */
static struct mcb * find_mcb_by_address(void * mem_ptr)
{
    struct mcb * mcb_ptr = allocated_mem_list;
    while (mcb_ptr != NULL){
        if(mcb_ptr->complete_mcb->begin_address == mem_ptr)
            break;
        mcb_ptr = mcb_ptr->next;
    }
    return mcb_ptr;
}

/**
 * @name insert_mcb_to_queue
 * @brief Inserts MCB to the queue
 *
 * @param  mcb_ptr  MCB Pointer
 * @param  type     The type of MCB
 * 
 */
static void insert_mcb_to_queue(struct mcb * mcb_ptr, enum mcb_type type)
{
    struct mcb ** target_list = &free_mem_list;
    if(type == allocated)
        target_list = &allocated_mem_list;
    
    //printf("*Debug* Insert: 1. %s \n", (type == free?"free":"alloc"));
    //printf("*Debug* Insert: 2. %X \n", *target_list);
    
    //Find previous free mcb
    struct mcb * prev_free_mcb = prev_adjacent_mcb(mcb_ptr);
    while(prev_free_mcb && prev_free_mcb->complete_mcb->type != type)
    {
        prev_free_mcb = prev_adjacent_mcb(prev_free_mcb);
    }
    if(prev_free_mcb) //If it is not inserting to first node.
    {
        mcb_ptr->prev = prev_free_mcb;
        mcb_ptr->next = prev_free_mcb->next;
        prev_free_mcb->next = mcb_ptr;
        if(mcb_ptr->next)
            mcb_ptr->next->prev = mcb_ptr;
    }
    else //If it is inserting to first node.
    {
        mcb_ptr->prev = NULL;
        mcb_ptr->next = *target_list;
        *target_list = mcb_ptr;
        if(mcb_ptr->next)
            mcb_ptr->next->prev = mcb_ptr;
    }
}

/**
 * @name remove_mcb_to_queue
 * @brief Removes MCB from the queue
 *
 * @param  mcb_ptr  MCB Pointer
 * @param  type     The type of MCB
 * 
 */
static void remove_mcb_from_queue(struct mcb * mcb_ptr, enum mcb_type type)
{
    struct mcb ** target_list = &free_mem_list;
    if(type == allocated)
        target_list = &allocated_mem_list;
        
    if(mcb_ptr->prev) //It is not first node.
    {
        mcb_ptr->prev->next = mcb_ptr->next;
    }
    else //It is first node.
    {
        *target_list = mcb_ptr->next;
    }
    
    if(mcb_ptr->next) //It is not last node.
    {
        mcb_ptr->next->prev = mcb_ptr->prev;
    }
    mcb_ptr->next = mcb_ptr->prev = NULL;
}

/**
 * @name init_heap
 * @brief Allocates all the memory for MPX
 *
 * @param  size  Size of heap in bytes
 * 
 */
void init_heap(u32int size) 
{
    start_of_memory = kmalloc(get_mcb_total_size(size));
    end_of_memory = start_of_memory + get_mcb_total_size(size);
    
    struct mcb * first_free_mem = (struct mcb *)start_of_memory;
    
    init_mem_block(first_free_mem, size, free);
    
    free_mem_list = first_free_mem;
    allocated_mem_list = NULL;

}

/**
 * @name show_mcb
 * @brief Displays the allocated or free memory block's address, 
 *      previous and next pointers, and block's size.
 * 
 * @param  mcb_ptr  MCB Pointer
 * 
 */
void show_mcb(struct mcb * mcb_ptr)
{
    if(!mcb_ptr)
        return;
    
    char * type_str_c = mcb_ptr->complete_mcb->type == allocated ? "   Allocated   \0" : 
                                                                 "      Free     \0" ;
    char * type_str_l = mcb_ptr->limited_mcb->type == allocated ? "   Allocated   \0" : 
                                                                 "      Free     \0" ;
    printf("\t\t  ______________________________________ \n");
    printf("%X\t |%X|%s|%X | \n", mcb_ptr, 
            mcb_ptr->prev, type_str_c, mcb_ptr->next);
    printf("\t\t |__________|_______________|___________| \n");
    printf("%X\t |\t\t   DATA  \t\t| \n", mcb_ptr->complete_mcb->begin_address);
        
    printf("\t\t |Size: %-25d Bytes | \n", mcb_ptr->complete_mcb->size);
    printf("\t\t |______________________________________| \n");
    printf("%X\t |%s %-15d Bytes | \n", mcb_ptr->limited_mcb, type_str_l, mcb_ptr->limited_mcb->size);
    printf("\t\t |______________________________________| \n");
}

/**
 * @name find_first_fit_mcb
 * @brief Finds the first block in the free_mem_list big enough to hold mem_size
 *
 * @param  mem_size     The size to be allocated from the heap
 * 
 * @return  The pointer to the MCB
 */
static struct mcb * find_first_fit_mcb(u32int mem_size)
{
    struct mcb * mcb_ptr = free_mem_list;
    u32int needed_size_2 = get_mcb_total_size(mem_size) + get_mcb_total_size(0);
    while (mcb_ptr){
        if(mcb_ptr->complete_mcb->size == mem_size)
        {
            return mcb_ptr;
        }
        else if((mcb_ptr->complete_mcb->size > mem_size)
            && (get_mcb_total_size(mcb_ptr->complete_mcb->size) >= needed_size_2))
        {
            return mcb_ptr;
        }
        mcb_ptr = mcb_ptr->next;
    }
    
    return NULL;
}

/**
 * @name mcb_allocate
 * @brief Allocates a memory block
 *
 * @param  mem_size     The MCB size to be allocated
 * 
 *
 * @return  Address to allocated MCB
 * @return  NULL if not enough space in free memory found
 */
void * mcb_allocate(u32int mem_size)
{
    struct mcb * first_fit_mcb = find_first_fit_mcb(mem_size);
    
    if(first_fit_mcb) //If we found the suitable memory space.
    {
        remove_mcb_from_queue(first_fit_mcb, free);
        u32int free_mcb_size = get_mcb_total_size(first_fit_mcb->complete_mcb->size);
        init_mem_block(first_fit_mcb, mem_size, allocated);
        insert_mcb_to_queue(first_fit_mcb, allocated);
        
        if(free_mcb_size > get_mcb_total_size(first_fit_mcb->complete_mcb->size))
        {//If the space we found is a litte biger.
            struct mcb * next_adja_mcb = next_adjacent_mcb(first_fit_mcb);
            u32int new_free_mem_size = free_mcb_size - get_mcb_total_size(first_fit_mcb->complete_mcb->size) - get_mcb_total_size(0);
            init_mem_block(next_adja_mcb, new_free_mem_size, free);
            insert_mcb_to_queue(next_adja_mcb, free);
        }
        
        return first_fit_mcb->complete_mcb->begin_address;
    }
    
    return NULL;
}

/**
 * @name mcb_free
 * @brief Frees a block of memory that was previously allocated
 *
 * @param  mcb_ptr     MCB Node pointer
 * 
 * @return  E_NOERROR   No Error found
 * @return  E_INVPARA   Invalid Parameter
 */
static error_t mcb_free(struct mcb * mcb_ptr)
{
    if(mcb_ptr == NULL)
        return E_NOERROR; //It is already free, return no error.
    
    //Remove from allocated memory queue first.
    remove_mcb_from_queue(mcb_ptr, allocated);
    
    //Insert to free memory queue.
    u32int new_size = mcb_ptr->complete_mcb->size;
    unsigned char is_found_adja = 0;
    
    //Case 1 : There is one or more adjacent free mcb
    {
        struct mcb * adja_mcb = prev_adjacent_mcb(mcb_ptr);
        if(adja_mcb && adja_mcb->complete_mcb->type == free)
        {
            remove_mcb_from_queue(adja_mcb, free);
            new_size = get_mcb_total_size(adja_mcb->complete_mcb->size) + get_mcb_total_size(mcb_ptr->complete_mcb->size)
                        - get_mcb_total_size(0);
            init_mem_block(adja_mcb, new_size, free);
            mcb_ptr = adja_mcb;
            is_found_adja = 1;
        }
        adja_mcb = next_adjacent_mcb(mcb_ptr);
        if(adja_mcb && adja_mcb->complete_mcb->type == free)
        {
            remove_mcb_from_queue(adja_mcb, free);
            new_size = get_mcb_total_size(adja_mcb->complete_mcb->size) + get_mcb_total_size(mcb_ptr->complete_mcb->size)
                        - get_mcb_total_size(0);
            init_mem_block(mcb_ptr, new_size, free);
            is_found_adja = 1;
        }
        if(is_found_adja)
            insert_mcb_to_queue(mcb_ptr, free);
    }
    
    //Case 2 : There is no adjacent free mcb
    if(!is_found_adja)
    {
        init_mem_block(mcb_ptr, new_size, free);
        insert_mcb_to_queue(mcb_ptr, free);
    }
    
    return E_NOERROR;
}

/**
 * @name show_allocated_mcb
 * @brief Displays all the allocated MCBs
 */
void show_allocated_mcb(){
    
    struct mcb * ptr = NULL;
    ptr = allocated_mem_list;
    
    while (ptr != NULL) {
        show_mcb(ptr);
        ptr = ptr->next;
    }
    
    printf("\n");
}

/**
 * @name show_free_mcb
 * @brief Displays all the free memory
 */
void show_free_mcb(){
    
    struct mcb * ptr = NULL;
    ptr = free_mem_list;
    
    while (ptr != NULL) {
        show_mcb(ptr);
        ptr = ptr->next;
    }
    
    printf("\n");
}

/**
 * @name show_all_mcb
 * @brief Displays all the free and allocated memory
 */
void show_all_mcb(){
    printf("Free list -> %X \n", free_mem_list);
    printf("allocated list -> %X \n", allocated_mem_list);
    
    struct mcb * ptr = (struct mcb *)start_of_memory;
    while (ptr != NULL){
        show_mcb(ptr);
        ptr = next_adjacent_mcb(ptr);
    }
    printf("\n");
}

/**
 * @name mcb_allocate_mpx
 * @brief Calls mcb_allocate to allocate memory block, used as parameter for sys_set_malloc in kmain.c
 *
 * @param  size     Size of block in bytes to allocate
 * 
 * @return Address of allocated MCB
 */
u32int mcb_allocate_mpx(u32int size)
{
    // printf("Testing student_malloc: Using mcb_allocate\n");
    return (u32int)mcb_allocate(size);
}

/**
 * @name mcb_free_mpx
 * @brief Calls mcb_free to free memory block, used as parameter for sys_set_free in kmain.c
 *
 * @param  mem_ptr    Memory Pointer
 * 
 * @return 0
 */
int mcb_free_mpx(void * mem_ptr)
{
    if(mem_ptr == NULL)
        return (int)E_NOERROR; //It is already free, return no error.
    
    struct mcb * mcb_ptr = find_mcb_by_address(mem_ptr);
    if(!mcb_ptr)
        return (int)E_INVPARA;
        
    return (int)(mcb_free(mcb_ptr));
}

/**
 * @name is_mcb_empty
 * @brief Checks if the heap is empty
 * 
 * @return 0 or 1 (true or false)
 */
int is_mcb_empty(){
    return (allocated_mem_list == NULL);
}

/**
 * @name shutdown_mcb.
 * @brief Shutdown the pcb during the shutdown procedure.
 * 
 * @return  0
 */
void shutdown_mcb()
{
    struct mcb * mcb_ptr = allocated_mem_list;
    
    while (mcb_ptr)
    {
        mcb_free(mcb_ptr);
        mcb_ptr = mcb_ptr->next;
    }
}

//#############################################################################
//Permanent User's Commands

/**
 * @name show_mcb_main.
 * @brief The function of show MCB for commhand.
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int show_mcb_main(int argc, char ** argv)
{
    
    if (argc != 3 )
    {
        printf("ERROR: Incorrect number of arguments. Please refer to \"mcb show --help\"\n");
        return 0;
    }
    else
    {
        if (!strcmp(argv[2], "--help"))
        {
            print_help(SHOWMCB);
        }
        else if (!strcmp(argv[2], "-free"))
        {
            show_free_mcb();
        }
        else if (!strcmp(argv[2], "-allocated"))
        {
            show_allocated_mcb();
        }
        else if (!strcmp(argv[2], "-all"))
        {
            show_all_mcb();
        }
        else
        {
            printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
        }
    }
    return 0;
}


//#############################################################################
//Temperary User's Commands

/**
 * @name is_digit
 * @brief Checks if input is a digit used for testing for module R5
 * 
 * @param ch    single character
 * @return 0 or 1 (true or false)
 */
static unsigned char is_digit(const char ch)
{
    return ('0' <= ch && ch <= '9');
}

/**
 * @name is_all_digit
 * @brief Checks if string is a digit used for testing for module R5
 * 
 * @param str_ptr    string array
 * @brief 0 or 1 (true or false)
 */
static unsigned char is_all_digit(const char * str_ptr)
{
    unsigned char result = 1;
    while(result && *str_ptr && (result = is_digit(*(str_ptr++))));
    return result;
}

/**
 * @name init_heap_main
 * @brief The main function of initate heap only used for testing in commhand for module R5 
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int init_heap_main(int argc, char ** argv) 
{
    if ( argc != 3 )
    {
        printf("ERROR: Incorrect number of arguments. Please refer to \"mcb init --help\"\n");
        return 0;
    }
    else
    {
        if (!strcmp(argv[2], "--help"))
        {
            print_help(INITMCB);
        }
        else if (is_all_digit(argv[2]))
        {
            u32int size = (u32int)atoi(argv[2]);
    
            init_heap(size);
    
            printf("Heap initialized!\n");
        }
        else
        {
            printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
        }
    }
    
    return 0;
}

/**
 * @name mcb_allocate_main.
 * @brief The main function of MCB allocate only used for testing in commhand for module R5
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int mcb_allocate_main( int argc, char ** argv )
{
    if ( argc != 3 )
    {
        printf("ERROR: Incorrect number of arguments. Please refer to \"mcb init --help\"\n");
        return 0;
    }
    else
    {
        if (!strcmp(argv[2], "--help"))
        {
            print_help(ALLOCMCB);
        }
        else if (is_all_digit(argv[2]))
        {
            u32int size = (u32int)atoi(argv[2]);
    
            void * alloc_addr = mcb_allocate(size);
    
            printf("MCB allocated %s error.\n", (alloc_addr ? "without" : "with"));
        }
        else
        {
            printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
        }
    }
    
    return 0;
}

/**
 * @name mcb_free_main.
 * @brief The main function of free MCB only used for testing in commhand for module R5
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int mcb_free_main(int argc, char ** argv)
{
    if (argc != 3 )
    {
        printf("ERROR: Incorrect number of arguments. Please refer to \"mcb free --help\"\n");
        return 0;
    }
    else
    {
        if (!strcmp(argv[2], "--help"))
        {
            print_help(FREEMCB);
        }
        else if (is_all_digit(argv[2]))
        {
            const unsigned int mcb_index = atoi(argv[2]);
            unsigned int i = 0;
            struct mcb * found_mcb = allocated_mem_list;
            for(i = 0; found_mcb && i < mcb_index - 1; i++)
            {
                // found_mcb = found_mcb->complete_mcb->next;
                found_mcb = found_mcb->next;
            }
            
            if(found_mcb)
            {
                error_t errno = mcb_free(found_mcb->complete_mcb->begin_address);
                printf("The mcb is free %s error.\n", (errno == E_NOERROR ? "without" : "with"));                

            }
            else
            {
                printf("ERROR: The mcb you want (with index %d) does not exist!\n\n", mcb_index);
            }
        }
        else
        {
            printf("ERROR: Invalid arugment, \"%s\", provided!\n\n", argv[2]);
        }
    }
    return 0;
}

/**
 * @name is_mcb_empty_main.
 * @brief The main function of is MCB empty only used for testing in commhand for module R5
 * 
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int is_mcb_empty_main(int argc, char **argv) {
    
    if (argc == 3 && !strcmp(argv[2], "--help"))
    {
        print_help(ISMCBEMPT);
    }
    else if(argc == 2)
    {
        if (is_mcb_empty())
        {
            printf("The MCB is empty.");
        }
        else if (is_mcb_empty())
        {
            printf("The MCB is allocated.");
        }
    }
    else
    {
        printf("ERROR: Incorrect number of arguments. Please refer to \"mcb free --help\"\n");
        return 0;
    }
    
    return 0;
}

