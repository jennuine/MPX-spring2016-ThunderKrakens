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


u32int start_of_memory;
u32int end_of_memory;

//struct mcb_list {
//    struct mcb *prev;
//    struct mcb *next;
//};

struct mcb * free_mem_list;
struct mcb * allocated_mem_list;

static u32int get_mcb_total_size(u32int mem_size)
{
    return (sizeof(struct mcb) + sizeof(struct cmcb) + mem_size + sizeof(struct lmcb));
}

static void init_mem_block(struct mcb * start_pos, u32int size, enum mcb_type type)
{
    void * start_pos_ptr = (void *)start_pos;
    start_pos->complete_mcb = (struct cmcb *)start_pos_ptr + sizeof(struct mcb);
    start_pos->limited_mcb = (struct lmcb *)(start_pos_ptr + sizeof(struct mcb) + sizeof(struct cmcb) + size);
    start_pos->complete_mcb->begin_address = (void *)(start_pos_ptr + sizeof(struct mcb) + sizeof(struct cmcb));
    
    start_pos->complete_mcb->size = start_pos->limited_mcb->size = size;
    start_pos->complete_mcb->type = start_pos->limited_mcb->type = type;
    
    start_pos->complete_mcb->prev = start_pos->complete_mcb->next = NULL;
}

void init_heap(u32int size) 
{
    start_of_memory = kmalloc(get_mcb_total_size(size));
    end_of_memory = start_of_memory + get_mcb_total_size(size);
    
    struct mcb * first_free_mem = (struct mcb *)start_of_memory;
    
    init_mem_block(first_free_mem, size, free);
    
    free_mem_list = first_free_mem;
    allocated_mem_list = NULL;
    //struct mcb * block = sys_alloc_mem(sizeof(struct mcb));
    //struct cmcb * complete = sys_alloc_mem(sizeof(struct cmcb));
    //struct lmcb * limited = sys_alloc_mem(sizeof(struct lmcb));

    //complete->type = free;
    //limited->type = free;
    //block->complete_mcb = complete;
    //block->limited_mcb = limited;

    // slides: 'put a cmcb at top of heap & lmcb at bottom both type free'
    //call make_heap ?
    
    //allocated_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    //allocated_mem_list.next = NULL;
    //allocated_mem_list.prev = NULL;
    
    //free_mem_list = sys_alloc_mem(sizeof(struct mcb_list));
    //free_mem_list.next = block;
    //free_mem_list.prev = NULL;
}
/*//commented temp for compilation... 
void * mcb_allocate(u32int size)
{
    u32int alloc_size = size + sizeof(struct cmcb) + sizeof(struct lmcb);
    
}
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
    printf("%X\t |%X|%s|%X | \n", mcb_ptr->complete_mcb, 
            mcb_ptr->complete_mcb->prev, type_str_c, mcb_ptr->complete_mcb->next);
    printf("\t\t |__________|_______________|___________| \n");
    printf("%X\t |\t\t   DATA  \t\t| \n", mcb_ptr->complete_mcb->begin_address);
    printf("\t\t |Size: %-25d Bytes | \n", mcb_ptr->complete_mcb->size);
    printf("\t\t |______________________________________| \n");
    printf("%X\t |%s %-15d Bytes | \n", mcb_ptr->limited_mcb, type_str_l, mcb_ptr->limited_mcb->size);
    printf("\t\t |______________________________________| \n");
}

static struct mcb * prev_adjacent_mcb(struct mcb * mcb_ptr)
{    
    struct lmcb * prev_lmcb = (struct lmcb *)((u32int)mcb_ptr - sizeof(struct lmcb));
    
    if((u32int)prev_lmcb <= start_of_memory)
        return NULL;
    
    struct mcb * prev_mcb = (struct mcb *)((u32int)mcb_ptr - get_mcb_total_size(prev_lmcb->size));
    
    return prev_mcb;
}

static struct mcb * next_adjacent_mcb(struct mcb * mcb_ptr)
{    
    struct mcb * next_mcb = (struct mcb *)((u32int)mcb_ptr + get_mcb_total_size(mcb_ptr->complete_mcb->size));
    
    if((u32int)next_mcb >= end_of_memory)
        return NULL;
    
    return next_mcb;
}

static struct mcb * find_mcb_by_address(void * mem_ptr)
{
    struct mcb * mcb_ptr = allocated_mem_list;
    while (mcb_ptr != NULL){
        if(mcb_ptr->complete_mcb->begin_address == mem_ptr)
            break;
        mcb_ptr = mcb_ptr->complete_mcb->next;
    }
    return mcb_ptr;
}

error_t mcb_free(void * mem_ptr)
{
    if(mem_ptr == NULL)
        return E_NOERROR; //It is already free, return no error.
    
    struct mcb * mcb_ptr = find_mcb_by_address(mem_ptr);
    if(!mcb_ptr)
        return E_INVPARA;
    
    //Remove from allocated memory queue first.
    if(mcb_ptr->complete_mcb->prev) //It is not first node.
    {
        mcb_ptr->complete_mcb->prev->complete_mcb->next = mcb_ptr->complete_mcb->next;
    }
    else //It is first node.
    {
        allocated_mem_list = mcb_ptr->complete_mcb->next;
    }
    
    if(mcb_ptr->complete_mcb->next) //It is not last node.
    {
        mcb_ptr->complete_mcb->next->complete_mcb->prev = mcb_ptr->complete_mcb->prev;
    }
    
    //Insert to free memory queue.
    u32int new_size = mcb_ptr->complete_mcb->size; 
    struct mcb * new_prev = NULL;
    struct mcb * new_next = NULL;
    unsigned char is_found_adja = 0;
    
    //Case 1 : There is one or more adjacent free mcb
    {
        struct mcb * adja_mcb = prev_adjacent_mcb(mcb_ptr);
        if(adja_mcb && adja_mcb->complete_mcb->type == free)
        {
            is_found_adja = 1;
            new_prev = adja_mcb->complete_mcb->prev;
            new_next = adja_mcb->complete_mcb->next;
            new_size = get_mcb_total_size(adja_mcb->complete_mcb->size) + get_mcb_total_size(mcb_ptr->complete_mcb->size)
                        - sizeof(struct mcb) - sizeof(struct cmcb) - sizeof(struct lmcb);
            init_mem_block(adja_mcb, new_size, free);
            mcb_ptr = adja_mcb;
            mcb_ptr->complete_mcb->prev = new_prev;
            mcb_ptr->complete_mcb->next = new_next;
        }
        adja_mcb = next_adjacent_mcb(mcb_ptr);
        if(adja_mcb && adja_mcb->complete_mcb->type == free)
        {
            is_found_adja = 1;
            new_prev = new_prev ? new_prev : adja_mcb->complete_mcb->prev;
            new_next = adja_mcb->complete_mcb->next;
            new_size = get_mcb_total_size(adja_mcb->complete_mcb->size) + get_mcb_total_size(mcb_ptr->complete_mcb->size)
                        - sizeof(struct mcb) - sizeof(struct cmcb) - sizeof(struct lmcb);
            init_mem_block(mcb_ptr, new_size, free);
            mcb_ptr->complete_mcb->prev = new_prev;
            mcb_ptr->complete_mcb->next = new_next;
        }
    }
    
    //Case 2 : There is no adjacent free mcb
    if(!is_found_adja)
    {
        //Find previous free mcb
        struct mcb * prev_free_mcb = prev_adjacent_mcb(mcb_ptr);
        while(prev_free_mcb && prev_free_mcb->complete_mcb->type != free)
        {
            prev_free_mcb = prev_adjacent_mcb(prev_free_mcb);
        }
        
        if(prev_free_mcb) //If it is not inserting to first node.
        {
            mcb_ptr->complete_mcb->prev = prev_free_mcb;
            mcb_ptr->complete_mcb->next = prev_free_mcb->complete_mcb->next;
            prev_free_mcb->complete_mcb->next = mcb_ptr;
            if(mcb_ptr->complete_mcb->next)
                mcb_ptr->complete_mcb->next->complete_mcb->prev = mcb_ptr;
        }
        else //If it is inserting to first node.
        {
            mcb_ptr->complete_mcb->prev = NULL;
            mcb_ptr->complete_mcb->next = free_mem_list;
            free_mem_list = mcb_ptr;
            if(mcb_ptr->complete_mcb->next)
                mcb_ptr->complete_mcb->next->complete_mcb->prev = mcb_ptr;
        }
    }
    return E_NOERROR;
}

void show_allocated_mcb(){
    struct mcb * ptr = NULL;
    ptr = allocated_mem_list;
    while (ptr != NULL){
        show_mcb(ptr);
        ptr = ptr->complete_mcb->next;
    }
    printf("\n");
}

void show_free_mcb(){
    struct mcb * ptr = NULL;
    ptr = free_mem_list;
    while (ptr != NULL){
        show_mcb(ptr);
        ptr = ptr->complete_mcb->next;
    }
    printf("\n");
}

void show_all_mcb(){
    struct mcb * ptr = (struct mcb *)start_of_memory;
    while (ptr != NULL){
        show_mcb(ptr);
        ptr = next_adjacent_mcb(ptr);
    }
    printf("\n");
}

//#############################################################################
//Permanent User's Commands

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
static unsigned char is_digit(const char ch)
{
    return ('0' <= ch && ch <= '9');
}
static unsigned char is_all_digit(const char * str_ptr)
{
    unsigned char result = 1;
    while(result && *str_ptr && (result = is_digit(*(str_ptr++)))) { }
    return result;
}

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
                found_mcb = found_mcb->complete_mcb->next;
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