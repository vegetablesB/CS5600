/*
 * @Author: Cao Ning
 * @Date: 2023-02-13 19:46:43
 * @FilePath: pm_heap.c
 * @Assignment: HW6 Build a Custom Heap
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include "pm_heap.h"

char pm_heap[PM_HEAP_SIZE];
BlockHeader *free_list = NULL;
int free_size = PM_HEAP_SIZE;

void *pm_malloc(size_t size)
{
    BlockHeader *current = free_list;

    while (current != NULL)
    {
        if (current->size >= size && current->is_free)
        {
            if (current->size > size + sizeof(BlockHeader))
            {
                BlockHeader *new_block = (BlockHeader *)((char *)current + size + sizeof(BlockHeader));
                new_block->size = current->size - size - sizeof(BlockHeader);
                new_block->next = current->next;
                new_block->is_free = 1;
                current->size = size;
                current->next = new_block;
                current->is_free = 0;
                free_size = free_size - size - sizeof(BlockHeader);
            }

            return (void *)(current + 1);
        }

        current = current->next;
    }

    return NULL;
}

void pm_free(void *ptr)
{
    BlockHeader *block = (BlockHeader *)ptr - 1;
    block->is_free = 1;
    free_size = free_size + block->size + sizeof(BlockHeader);

    BlockHeader *current = free_list;
    BlockHeader *previous = NULL;

    while (current != NULL)
    {
        if (current > block)
        {
            break;
        }

        previous = current;
        current = current->next;
    }

    if (previous != NULL)
    {
        previous->next = block;
    }

    block->next = current;
    merge_free_blocks();
}

// Merge the blank blocks
void merge_free_blocks()
{
    BlockHeader *current = free_list;
    BlockHeader *previous = NULL;

    while (current != NULL)
    {
        if (current->is_free)
        {
            if (previous != NULL && previous->is_free)
            {
                previous->next = current->next;
                previous->size = previous->size + current->size + sizeof(BlockHeader);
            }
        }

        previous = current;
        current = current->next;
    }
}

// Return the size of the free list
int pm_get_heap_free_size()
{
    return free_size;
}

// Initialize the free list with the first block header
void initialize_pm_heap()
{
    // Initialize the free list
    // ignore the first block header memory usage
    free_list = (BlockHeader *)pm_heap;
    free_list->size = PM_HEAP_SIZE - (int)sizeof(BlockHeader);
    free_list->next = NULL;
    free_list->is_free = 1;
    free_size = PM_HEAP_SIZE - (int)sizeof(BlockHeader);
}
