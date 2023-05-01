/*
 * @Author: Cao Ning
 * @Date: 2023-02-13 19:47:37
 * @FilePath: pm_heap.h
 * @Assignment: HW6 Build a Custom Heap
 * @Course: CS5600 Spring 2023
 */
#include <stddef.h> // size_t is not built-in type in C99

#ifndef PM_HEAP_H
#define PM_HEAP_H

#define PM_HEAP_SIZE (10 * 1024 * 1024)

typedef struct BlockHeader
{
    size_t size;
    struct BlockHeader *next;
    int is_free;
} BlockHeader;

void *pm_malloc(size_t size);
void pm_free(void *ptr);
void merge_free_blocks();
int pm_get_heap_free_size();
void initialize_pm_heap();

#endif
