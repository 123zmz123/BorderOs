#include "memory.h"
#include "debug.h"
#include "print.h"
#include "stddef.h"

static struct Page free_memory;
extern char end;

static void free_region(uint64_t v, uint64_t e)
{
    for (uint64_t start = PA_UP(v); start + PAGE_SIZE <= e; start += PAGE_SIZE) 
    {
        if ((start + PAGE_SIZE) <= MEMORY_END)
        {
            kfree(start);
        }
    }
}
void kfree(uint64_t v)
{
    ASSERT(v % PAGE_SIZE == 0);
    ASSERT(v >= (uint64_t)&end);
    ASSERT(v + PAGE_SIZE <= MEMORY_END);
    /*free_memory->page_adress->next_page_address->next_page_address->........*/
    struct Page *page_address = (struct Page*)v;
    page_address->next = free_memory.next;
    free_memory.next = page_address;
}
void* kalloc(void)
{
    struct Page *page_address = free_memory.next;
    if (page_address != NULL)
    {
        ASSERT((uint64_t)page_address % PAGE_SIZE == 0);
        ASSERT((uint64_t)page_address >= (uint64_t)&end);
        ASSERT((uint64_t)page_address + PAGE_SIZE <= MEMORY_END);

        free_memory.next = page_address->next;
    }
    return page_address;
}

void checkmm(void)
{
    struct Page *v = free_memory.next;
    uint64_t size = 0;
    uint64_t i = 0;
    while (v != NULL)
    {
        size += PAGE_SIZE;
        printk("%d base is %x \r\n", i++, v);
        v = v->next;
    }
    printk("memory size is %u \r\n", size / 1024 / 1024);
}

void init_memory(void)
{
    //setup the page table from end of the kernl to memory boundry
    free_region((uint64_t)&end, MEMORY_END);
    checkmm();
}
