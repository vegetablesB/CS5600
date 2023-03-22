#include "pm_heap.h"
#include <limits.h>

static char pm_heap[PM_HEAP_SIZE];                          // memory heap
static pm_page_table_entry pm_page_table[2 * PM_NUM_PAGES]; // size = memory + disk
pthread_mutex_t pm_mutex;
static int timestamp_counter = 0; // LRU timestamp
FILE *disk_file;

// Initialize the page table and disk file
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

// Find the least recently used page based on the minimum timestamp
int find_lru_page(void)
{
    int min_timestamp = INT_MAX;
    int lru_page = 0;
    for (int i = 0; i < PM_NUM_PAGES; i++)
    {
        if (!pm_page_table[i].free && pm_page_table[i].timestamp < min_timestamp && !pm_page_table[i].present)
        {
            min_timestamp = pm_page_table[i].timestamp;
            lru_page = i;
        }
    }
    return lru_page;
}

// Find the physical address of the page in physical memory
int find_physical_address()
{
    // size_t phy_address = SIZE_MAX;
    int index = -1;
    for (int i = 0; i < PM_NUM_PAGES; i++)
    {
        int flag = 0;
        for (int j = 0; j < 2 * PM_NUM_PAGES; j++)
        {

            if (pm_page_table[j].physical_address == (size_t)(i * PM_PAGE_SIZE) && !pm_page_table[j].free)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            // phy_address = (size_t)(i * PM_PAGE_SIZE);
            index = i;
            break;
        }
    }
    return index;
}

// Swap out the page to disk
void *swap_out(int lru_page)
{
    fseek(disk_file, pm_page_table[lru_page].disk_offset, SEEK_SET);
    fwrite(pm_heap + pm_page_table[lru_page].physical_address, 1, PM_PAGE_SIZE, disk_file);
    printf(" Swapping out page %d, contents: %s, pointing to physical address: %d\n", (int)lru_page, pm_heap + pm_page_table[lru_page].physical_address, (int)pm_page_table[lru_page].physical_address / (int)PM_PAGE_SIZE);
    pm_page_table[lru_page].free = 1;
    pm_page_table[lru_page].size = 0;
    pm_page_table[lru_page].present = 1; // ????
    return (void *)(pm_page_table[lru_page].physical_address);
}

// Swap in the page from disk, to the destination based on the second argument
void swap_in(int disk_page, int lru_page)
{
    fseek(disk_file, pm_page_table[disk_page].disk_offset, SEEK_SET);
    fread(pm_heap + pm_page_table[lru_page].physical_address, 1, PM_PAGE_SIZE, disk_file);
    printf(" Swapping in page %d, contents: %s, pointing to physical address: %d\n", (int)disk_page, pm_heap + pm_page_table[lru_page].physical_address, (int)pm_page_table[lru_page].physical_address / (int)PM_PAGE_SIZE);
    pm_page_table[disk_page].free = 0;
    pm_page_table[disk_page].size = PM_PAGE_SIZE;
    pm_page_table[disk_page].timestamp = ++timestamp_counter;
    pm_page_table[disk_page].present = 0;
}

// Allocate a page of memory
size_t pm_malloc(size_t size)
{
    pthread_mutex_lock(&pm_mutex);

    int free_page = 2 * PM_NUM_PAGES - 1;
    int free_page_count = 0;
    size_t empty_address = 0;
    // find non free page num
    for (int i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        if (!pm_page_table[i].free)
        {
            free_page_count++;
        }
    }
    // printf("\nFree page count: %d\n", (int)free_page_count);
    if (free_page_count >= PM_NUM_PAGES)
    {
        int lru_page = find_lru_page();
        empty_address = (size_t)swap_out(lru_page);
    }

    // Find a new free page table entry for the new page, that is not present
    for (int i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        if (pm_page_table[i].free && !pm_page_table[i].present)
        {
            free_page = i;
            break;
        }
    }

    if (empty_address != 0)
    {
        pm_page_table[free_page].physical_address = empty_address;
    }
    else
    {
        // find a free physical page based on which is used
        // pm_page_table[free_page].physical_address = find_physical_address();
        int index = find_physical_address();
        if (index == -1)
        {
            printf("\nNo free physical page\n");
            return -1;
        }
        pm_page_table[free_page].physical_address = (size_t)(index * PM_PAGE_SIZE);
    }
    pm_page_table[free_page].free = 0;
    pm_page_table[free_page].size = size;
    pm_page_table[free_page].timestamp = ++timestamp_counter;
    pthread_mutex_unlock(&pm_mutex);

    return free_page;
}

// Free a page of memory
void pm_free(int virtual_address)
{
    pthread_mutex_lock(&pm_mutex);
    if (virtual_address < 2 * PM_NUM_PAGES)
    {
        // if in disk, write a page 0 to disk
        if (pm_page_table[virtual_address].present == 1)
        {
            fseek(disk_file, pm_page_table[virtual_address].disk_offset, SEEK_SET);
            char *zero = (char *)calloc(PM_PAGE_SIZE, sizeof(char));
            fwrite(zero, 1, PM_PAGE_SIZE, disk_file);
            free(zero);
        }
        else if (pm_page_table[virtual_address].present == 0 && pm_page_table[virtual_address].free == 0)
        {
            // if in memory, write a page 0 to memory
            char *zero = (char *)calloc(PM_PAGE_SIZE, sizeof(char));
            memcpy(pm_heap + pm_page_table[virtual_address].physical_address, zero, PM_PAGE_SIZE);
            free(zero);
        }
        pm_page_table[virtual_address].free = 1;
        pm_page_table[virtual_address].size = 0;
        pm_page_table[virtual_address].present = 0;
        pm_page_table[virtual_address].timestamp = 0;
    }

    pthread_mutex_unlock(&pm_mutex);
}

// Write data to a page of memory
void *pm_write(int virtual_address, char *data, size_t size)
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
    printf(" Writing to page %d, contents: %s, pointing to physical address: %d\n", (int)virtual_address, pm_heap + pm_page_table[virtual_address].physical_address, (int)pm_page_table[virtual_address].physical_address / (int)PM_PAGE_SIZE);
    pthread_mutex_unlock(&pm_mutex);
    return (void *)(pm_heap + pm_page_table[virtual_address].physical_address);
}

// Read data from a page of memory
void *pm_read(int virtual_address)
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
            int free_page_count = 0;

            for (int i = 0; i < 2 * PM_NUM_PAGES; i++)
            {
                if (pm_page_table[i].free && !pm_page_table[i].present)
                {
                    free_page_count++;
                }
            }

            if (free_page_count < PM_NUM_PAGES)
            {
                int lru_page = find_lru_page();
                swap_out(lru_page);
                swap_in(virtual_address, lru_page);
            }
            else
            {
                // find a free physical page based on which is used
                int destination_free_page = find_physical_address();
                if (destination_free_page == -1)
                {
                    printf("\nNo free physical page\n");
                    return NULL;
                }
                swap_in(virtual_address, destination_free_page);
            }
        }
        else
        {
            // in memory, not present, update timestamp
            pm_page_table[virtual_address].timestamp = ++timestamp_counter;
        }
        // printf page number
        printf(" Reading page %d, contents: %s, from physical address %d_____\n", virtual_address, pm_heap + pm_page_table[virtual_address].physical_address, (int)pm_page_table[virtual_address].physical_address / (int)PM_PAGE_SIZE);
        pthread_mutex_unlock(&pm_mutex);

        return (void *)(pm_heap + pm_page_table[virtual_address].physical_address);
    }

    pthread_mutex_unlock(&pm_mutex);
    return NULL;
}

// Find all pages in disk
void find_page_in_disk()
{
    pthread_mutex_lock(&pm_mutex);
    int count = 0;
    for (int i = 0; i < 2 * PM_NUM_PAGES; i++)
    {
        if (pm_page_table[i].present)
        {
            printf("\nPage %d is in disk\n", (int)i);
            count++;
        }
    }
    if (count == 0)
    {
        printf("\nNo page in disk\n");
    }
    printf("\nThere are %d pages in disk\n", count);

    pthread_mutex_unlock(&pm_mutex);
}
