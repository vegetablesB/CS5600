#include "pm_heap.h"
#include <stdint.h>
#include <pthread.h>

static char pm_heap[PM_HEAP_SIZE];
static pm_page_table_entry pm_page_table[PM_NUM_PAGES];
pthread_mutex_t pm_mutex;

void pm_init(void)
{
    pthread_mutex_init(&pm_mutex, NULL);
    for (int i = 0; i < PM_NUM_PAGES; i++)
    {
        pm_page_table[i].free = 1;
        pm_page_table[i].size = 0;
    }
}

void *pm_malloc(size_t size)
{
    // Round up the size to a multiple of the page size
    size_t num_pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;

    // Lock the heap mutex
    pthread_mutex_lock(&pm_mutex);

    size_t contiguous_pages = 0;
    size_t start_page = 0;
    for (size_t i = 0; i < PM_NUM_PAGES; i++)
    {
        if (pm_page_table[i].free)
        {
            if (contiguous_pages == 0)
            {
                start_page = i;
            }
            contiguous_pages++;

            if (contiguous_pages == num_pages)
            {
                break;
            }
        }
        else
        {
            contiguous_pages = 0;
        }
    }

    if (contiguous_pages == num_pages)
    {
        for (size_t i = start_page; i < start_page + num_pages; i++)
        {
            pm_page_table[i].free = 0;
            pm_page_table[i].size = PM_PAGE_SIZE;
        }

        // Unlock the heap mutex
        pthread_mutex_unlock(&pm_mutex);

        return (void *)(pm_heap + start_page * PM_PAGE_SIZE);
    }

    // Unlock the heap mutex
    pthread_mutex_unlock(&pm_mutex);
    return NULL;
}

void pm_free(void *ptr)
{
    // Check if the pointer is NULL
    if (ptr == NULL)
    {
        return;
    }

    // Lock the heap mutex
    pthread_mutex_lock(&pm_mutex);

    size_t page_index = ((uintptr_t)ptr - (uintptr_t)pm_heap) / PM_PAGE_SIZE;

    while (page_index < PM_NUM_PAGES && pm_page_table[page_index].free == 0)
    {
        pm_page_table[page_index].free = 1;
        pm_page_table[page_index].size = 0;
        page_index++;
    }

    // Unlock the heap mutex
    pthread_mutex_unlock(&pm_mutex);
}
