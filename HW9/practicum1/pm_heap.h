#ifndef PM_HEAP_H
#define PM_HEAP_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PM_HEAP_SIZE (10 * 4096)                   // 40KB heap size
#define PM_PAGE_SIZE 4096                          // 4KB page size
#define PM_NUM_PAGES (PM_HEAP_SIZE / PM_PAGE_SIZE) // 10 pages
#define DISK_SIZE (10 * 4096)                      // 40KB disk size
#define DISK_FILE "disk.bin"

typedef struct pm_page_table_entry
{
    size_t size;             // size of the data in the page
    size_t physical_address; // physical address offset of the page
    int timestamp;           // LRU timestamp
    int free;                // 1 if the page is free, 0 if it is allocated
    int present;             // 1 if the page is in disk, 0 if it is on memory
    long disk_offset;        // offset in the disk file where the page is stored
} pm_page_table_entry;

void pm_init(void);
size_t pm_malloc(size_t size);
void pm_free(int virtual_address);
void *pm_write(int virtual_address, char *data, size_t size);
void *pm_read(int virtual_address);
void find_page_in_disk();

#endif
