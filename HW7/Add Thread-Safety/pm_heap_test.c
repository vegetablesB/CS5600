/*
 * @Author: Cao Ning
 * @Date: 2023-02-13 21:08:56
 * @FilePath: pm_heap_test.c
 * @Assignment: HW6 Build a Custom Heap
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pm_heap.h"

#define PAGE_SIZE 4096
#define BLOCK_SIZE 1024

int main()
{
    // The size of pm_block_header is 24 bytes, which is 0x18 in hex
    // The size of one page is 4096 bytes, which is 0x1000 in hex
    printf("The size of pm_block_header is %zu bytes, which is 0x18 in hex\n", sizeof(pm_block_header));
    printf("The size of one page is %d bytes, which is 0x1000 in hex\n\n", PAGE_SIZE);

    pm_init();
    int *int_ptr = (int *)pm_malloc(sizeof(int));
    *int_ptr = 42;
    printf("Allocated int at %p, value = %d\n", int_ptr, *int_ptr);
    printf("Allocated an integer: %d\n\n", *int_ptr);

    // Test allocation of a character
    char *char_ptr = (char *)pm_malloc(sizeof(char));
    *char_ptr = 'a';
    printf("Allocated char at %p, value = %c\n", char_ptr, *char_ptr);
    printf("%p = %p + 0x18 + 0x1000\n", char_ptr, int_ptr);
    printf("Allocated a character: %c\n", *char_ptr);

    printf("\nFree int and char\n\n");
    pm_free(int_ptr);
    pm_free(char_ptr);

    // Test allocation of a character list that requires more than one page
    size_t size = 5 * PM_PAGE_SIZE;
    char *large_char_list = (char *)pm_malloc(size);
    // memset(large_char_list, 'b', size);

    printf("Allocated a character list of size %zu, which is 0x5000\n", size);
    printf("Allocated address at %p\n\n", large_char_list);

    // Test allocation of a list of characters
    size_t list_size = 10;
    char *char_list = (char *)pm_malloc(list_size * sizeof(char));
    for (int i = 0; i < (int)list_size; i++)
    {
        char_list[i] = 'c' + i;
    }

    printf("Allocated a list of characters: ");
    for (int i = 0; i < (int)list_size; i++)
    {
        printf("%c ", char_list[i]);
    }
    printf("\n");
    printf("Allocated int at %p\n", char_list);
    printf("%p = %p + 0x18 + 0x5000\n", char_list, large_char_list);
    pm_free(large_char_list);
    pm_free(char_list);

    printf("Test passed\n");

    return 0;
}
