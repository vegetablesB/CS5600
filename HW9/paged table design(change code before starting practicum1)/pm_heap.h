#include <stddef.h>

#ifndef PM_HEAP_H
#define PM_HEAP_H

#include <stddef.h>

#define PM_HEAP_SIZE (10 * 1024 * 1024) // 10MB heap size
#define PM_PAGE_SIZE 4096               // 4KB page size
#define PM_NUM_PAGES (PM_HEAP_SIZE / PM_PAGE_SIZE)

typedef struct pm_page_table_entry
{
    int free;
    size_t size;
} pm_page_table_entry;

void pm_init(void);
void *pm_malloc(size_t size);
void pm_free(void *ptr);

#endif
