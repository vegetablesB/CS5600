/*
 * @Author: Cao Ning
 * @Date: 2023-02-26 13:46:01
 * @FilePath: pm_heap.c
 * @Assignment: HW7 Add Thread-Safety
 * @Course: CS5600 Spring 2023
 */
#include "pm_heap.h"
#include <stdint.h>
#include <pthread.h>

static char pm_heap[PM_HEAP_SIZE];
static pm_block_header *pm_free_list;
pthread_mutex_t pm_mutex;

void pm_init(void)
{
    pthread_mutex_init(&pm_mutex, NULL);
    pm_block_header *initial_block = (pm_block_header *)pm_heap;
    initial_block->size = PM_HEAP_SIZE - sizeof(pm_block_header);
    initial_block->free = 1;
    initial_block->next = NULL;
    pm_free_list = initial_block;
}

void *pm_malloc(size_t size)
{
    // Round up the size to a multiple of the page size
    size_t num_pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;
    size = num_pages * PM_PAGE_SIZE;

    // Lock the heap mutex
    pthread_mutex_lock(&pm_mutex);

    // Find a free block of the required size
    pm_block_header *prev_block = NULL;
    pm_block_header *block = pm_free_list;
    while (block != NULL)
    {
        if (block->size >= size)
        {
            // Found a suitable block
            block->free = 0;
            if (block->size > size + sizeof(pm_block_header))
            {
                // Split the block
                pm_block_header *new_block = (pm_block_header *)((char *)block + size + sizeof(pm_block_header));
                new_block->size = block->size - size - sizeof(pm_block_header);
                new_block->free = 1;
                new_block->next = block->next;
                block->next = new_block;
            }
            if (prev_block != NULL)
            {
                prev_block->next = block->next;
            }
            else
            {
                pm_free_list = block->next;
            }

            // Unlock the heap mutex
            pthread_mutex_unlock(&pm_mutex);

            return (void *)((char *)block + sizeof(pm_block_header));
        }
        prev_block = block;
        block = block->next;
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
    pm_block_header *block = (pm_block_header *)((char *)ptr - sizeof(pm_block_header));
    pm_block_header *prev_block = NULL;
    pm_block_header *next_block = NULL;

    // Check if the block is already free
    if (block->free == 1)
    {
        // Unlock the heap mutex
        pthread_mutex_unlock(&pm_mutex);
        return;
    }

    block->free = 1;
    pm_block_header *cur_block = pm_free_list;
    while (cur_block)
    {
        if (cur_block < block)
        {
            prev_block = cur_block;
            cur_block = cur_block->next;
        }
        else if (cur_block > block)
        {
            next_block = cur_block;
            break;
        }
        else
        {
            break;
        }
    }

    if (prev_block && (char *)prev_block + sizeof(pm_block_header) + prev_block->size == (char *)block)
    {
        prev_block->size += sizeof(pm_block_header) + block->size;
        block = prev_block;
    }
    if (next_block && (char *)block + sizeof(pm_block_header) + block->size == (char *)next_block)
    {
        block->size += sizeof(pm_block_header) + next_block->size;
        block->next = next_block->next;
    }
    else
    {
        block->next = next_block;
    }

    if (!prev_block)
    {
        pm_free_list = block;
    }
    else if ((char *)prev_block + sizeof(pm_block_header) + prev_block->size == (char *)block)
    {
        prev_block->next = block->next;
    }
    else
    {
        prev_block->next = block;
    }
    // Unlock the heap mutex
    pthread_mutex_unlock(&pm_mutex);
}
