#include "pm_heap.h"
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char pm_heap[PM_HEAP_SIZE];                          // memory heap
static pm_page_table_entry pm_page_table[2 * PM_NUM_PAGES]; // size = memory + disk
pthread_mutex_t pm_mutex;
static size_t timestamp_counter = 0; // LRU timestamp
FILE *disk_file;

void pm_init(void)
{
    pthread_mutex_init(&pm_mutex, NULL);
    for (int i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        pm_page_table[i].free = 1;
        pm_page_table[i].size = 0;
        pm_page_table[i].timestamp = 0;
        pm_page_table[i].physical_address = (i % PM_NUM_PAGES) * PM_PAGE_SIZE;
        pm_page_table[i].disk_offset = i * PM_PAGE_SIZE;
        pm_page_table[i].present = 0;
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
        if (!pm_page_table[i].free && pm_page_table[i].timestamp < min_timestamp && !pm_page_table[i].present)
        {
            min_timestamp = pm_page_table[i].timestamp;
            lru_page = i;
        }
    }
    return lru_page;
}

void *swap_out(size_t lru_page)
{
    fseek(disk_file, pm_page_table[lru_page].disk_offset, SEEK_SET);
    fwrite(pm_heap + pm_page_table[lru_page].physical_address, 1, PM_PAGE_SIZE, disk_file);
    printf("\nSwapping out page %d, contents: %s\n", (int)lru_page, pm_heap + pm_page_table[lru_page].physical_address);
    pm_page_table[lru_page].free = 1;
    pm_page_table[lru_page].size = 0;
    pm_page_table[lru_page].present = 1; // ????
    return (void *)(pm_page_table[lru_page].physical_address);
}

void swap_in(size_t disk_page, size_t lru_page)
{
    fseek(disk_file, pm_page_table[disk_page].disk_offset, SEEK_SET);
    fread(pm_heap + pm_page_table[lru_page].physical_address, 1, PM_PAGE_SIZE, disk_file);
    printf("\nSwapping in page %d, contents: %s\n", (int)disk_page, pm_heap + pm_page_table[lru_page].physical_address);
    pm_page_table[disk_page].free = 0;
    pm_page_table[disk_page].size = PM_PAGE_SIZE;
    pm_page_table[disk_page].timestamp = ++timestamp_counter;
    pm_page_table[disk_page].present = 0;
}

size_t pm_malloc(size_t size)
{
    pthread_mutex_lock(&pm_mutex);

    size_t free_page = 2 * PM_NUM_PAGES - 1;
    size_t free_page_count = 0;
    size_t empty_address = 0;
    // find non free page num
    for (size_t i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        if (!pm_page_table[i].free)
        {
            free_page_count++;
        }
    }
    // printf("\nFree page count: %d\n", (int)free_page_count);
    if (free_page_count >= PM_NUM_PAGES)
    {
        size_t lru_page = find_lru_page();
        printf("\nLRU page: %d\n", (int)lru_page);
        empty_address = (size_t)swap_out(lru_page);
    }

    // Find a new free page table entry for the new page, that is not present
    for (size_t i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        if (pm_page_table[i].free && !pm_page_table[i].present)
        {
            free_page = i;
            printf("\nFree page: %d\n", (int)free_page);
            break;
        }
    }

    pm_page_table[free_page].free = 0;
    pm_page_table[free_page].size = size;
    pm_page_table[free_page].timestamp = ++timestamp_counter;
    pm_page_table[free_page].disk_offset = free_page * PM_PAGE_SIZE;
    if (empty_address != 0)
    {
        pm_page_table[free_page].physical_address = empty_address;
    }
    else
    {
        pm_page_table[free_page].physical_address = (free_page % PM_NUM_PAGES) * PM_PAGE_SIZE;
    }

    pthread_mutex_unlock(&pm_mutex);

    return free_page;
}

void pm_free(size_t virtual_address)
{
    pthread_mutex_lock(&pm_mutex);
    if (virtual_address < 2 * PM_NUM_PAGES)
    {
        pm_page_table[virtual_address].free = 1;
        pm_page_table[virtual_address].size = 0;
        pm_page_table[virtual_address].present = 0;
    }

    pthread_mutex_unlock(&pm_mutex);
}

void *pm_write(size_t virtual_address, char *data, size_t size)
{
    if (virtual_address >= 2 * PM_NUM_PAGES)
    {
        return NULL;
    }
    pthread_mutex_lock(&pm_mutex);

    if (size <= pm_page_table[virtual_address].size)
    {
        memcpy(pm_heap + pm_page_table[virtual_address].physical_address, data, size);
        // pm_page_table[virtual_address].present = 1;
    }
    else
    {
        fprintf(stderr, "Error: Writing beyond allocated size\n");
        pthread_mutex_unlock(&pm_mutex);
        return NULL;
    }

    pthread_mutex_unlock(&pm_mutex);
    return (void *)(pm_heap + pm_page_table[virtual_address].physical_address);
}

void *pm_read(size_t virtual_address)
{
    if (virtual_address >= 2 * PM_NUM_PAGES)
    {
        return NULL;
    }

    pthread_mutex_lock(&pm_mutex);

    if (!pm_page_table[virtual_address].free)
    {
        if (pm_page_table[virtual_address].present)
        {
            size_t free_page_count = 0;

            for (size_t i = 0; i < 2 * PM_NUM_PAGES; i++)
            {
                if (pm_page_table[i].free && !pm_page_table[i].present)
                {
                    free_page_count++;
                }
            }

            if (free_page_count < PM_NUM_PAGES)
            {
                size_t lru_page = find_lru_page();
                swap_out(lru_page);
                swap_in(virtual_address, lru_page);
            }
            else
            {
                swap_in(virtual_address, virtual_address);
            }
        }
        else
        {
            // in memory, not present, update timestamp
            pm_page_table[virtual_address].timestamp = ++timestamp_counter;
        }
        // printf page number
        printf("\nReading page %d, contents: %s\n", (int)virtual_address, pm_heap + pm_page_table[virtual_address].physical_address);
        pthread_mutex_unlock(&pm_mutex);

        return (void *)(pm_heap + pm_page_table[virtual_address].physical_address);
    }

    pthread_mutex_unlock(&pm_mutex);
    return NULL;
}
