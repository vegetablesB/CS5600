#include "pm_heap.h"
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char pm_heap[PM_HEAP_SIZE];
static pm_page_table_entry pm_page_table[PM_NUM_PAGES];
pthread_mutex_t pm_mutex;
static size_t timestamp_counter = 0;
FILE *disk_file;

void pm_init(void)
{
    pthread_mutex_init(&pm_mutex, NULL);
    for (int i = 0; i < PM_NUM_PAGES; i++)
    {
        pm_page_table[i].free = 1;
        pm_page_table[i].size = 0;
        pm_page_table[i].timestamp = 0;
        pm_page_table[i].disk_offset = i * PM_PAGE_SIZE;
    }
    disk_file = fopen(DISK_FILE, "wb+");
    if (!disk_file)
    {
        perror("Unable to open disk file");
        exit(EXIT_FAILURE);
    }
}

size_t find_lru_page(void)
{
    size_t min_timestamp = SIZE_MAX;
    size_t lru_page = 0;
    for (size_t i = 0; i < PM_NUM_PAGES; i++)
    {
        if (!pm_page_table[i].free && pm_page_table[i].timestamp < min_timestamp)
        {
            min_timestamp = pm_page_table[i].timestamp;
            lru_page = i;
        }
    }
    return lru_page;
}

void swap_out(size_t lru_page)
{
    pm_page_table[lru_page].disk_offset = ftell(disk_file);
    fwrite(pm_heap + lru_page * PM_PAGE_SIZE, 1, PM_PAGE_SIZE, disk_file);
    // print the contents of the page which is going to be swapped out
    printf("\nSwapping out page %d, contents: %s\n", (int)lru_page, pm_heap + lru_page * PM_PAGE_SIZE);
    pm_page_table[lru_page].free = 1;
    pm_page_table[lru_page].size = 0;
}

void swap_in(size_t lru_page, size_t new_page)
{
    fseek(disk_file, pm_page_table[lru_page].disk_offset, SEEK_SET);
    fread(pm_heap + new_page * PM_PAGE_SIZE, 1, PM_PAGE_SIZE, disk_file);
    pm_page_table[new_page].free = 0;
    pm_page_table[new_page].size = PM_PAGE_SIZE;
    pm_page_table[new_page].timestamp = ++timestamp_counter;
    pm_page_table[new_page].disk_offset = pm_page_table[lru_page].disk_offset;
}

void *pm_malloc(size_t size)
{
    size_t num_pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;

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

    if (contiguous_pages != num_pages)
    {
        printf("contiguous_pages is %d", (int)contiguous_pages);
        // Swap out the LRU page and swap in the required page
        size_t lru_page = find_lru_page();
        size_t new_page = start_page;
        swap_out(lru_page);
        swap_in(lru_page, new_page);
        start_page = new_page;
        contiguous_pages = num_pages;
    }

    if (contiguous_pages == num_pages)
    {
        for (size_t i = start_page; i < start_page + num_pages; i++)
        {
            pm_page_table[i].free = 0;
            pm_page_table[i].size = PM_PAGE_SIZE;
            pm_page_table[i].timestamp = ++timestamp_counter;
        }

        pthread_mutex_unlock(&pm_mutex);

        return (void *)(pm_heap + start_page * PM_PAGE_SIZE);
    }

    pthread_mutex_unlock(&pm_mutex);
    return NULL;
}

void pm_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    pthread_mutex_lock(&pm_mutex);

    size_t page_index = ((uintptr_t)ptr - (uintptr_t)pm_heap) / PM_PAGE_SIZE;

    while (page_index < PM_NUM_PAGES && pm_page_table[page_index].free == 0)
    {
        pm_page_table[page_index].free = 1;
        pm_page_table[page_index].size = 0;
        page_index++;
    }

    pthread_mutex_unlock(&pm_mutex);
}
