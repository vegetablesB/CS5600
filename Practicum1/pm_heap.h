#ifndef PM_HEAP_H
#define PM_HEAP_H

#include <stddef.h>

#define PM_HEAP_SIZE (10 * 4096) // 40KB heap size
#define PM_PAGE_SIZE 4096        // 4KB page size
#define PM_NUM_PAGES (PM_HEAP_SIZE / PM_PAGE_SIZE)
#define DISK_SIZE (10 * 4096) // 40KB disk size
#define DISK_FILE "disk.bin"

typedef struct pm_page_table_entry
{
    int free;
    size_t size;
    size_t timestamp; // LRU timestamp
    long disk_offset;
} pm_page_table_entry;

void pm_init(void);
void *pm_malloc(size_t size);
void pm_free(void *ptr);
char pm_read_char(void *ptr);

#endif
