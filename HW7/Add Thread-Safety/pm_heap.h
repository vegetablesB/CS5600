/*
 * @Author: Cao Ning
 * @Date: 2023-02-26 13:46:01
 * @FilePath: pm_heap.h
 * @Assignment: HW7 Add Thread-Safety
 * @Course: CS5600 Spring 2023
 */
#include <stddef.h>

#ifndef PM_HEAP_H
#define PM_HEAP_H

#include <stddef.h>

#define PM_HEAP_SIZE (10 * 1024 * 1024) // 10MB heap size
#define PM_PAGE_SIZE 4096               // 4KB page size

typedef struct pm_block_header
{
    size_t size;
    int free;
    struct pm_block_header *next;
} pm_block_header;

void pm_init(void);
void *pm_malloc(size_t size);
void pm_free(void *ptr);

#endif