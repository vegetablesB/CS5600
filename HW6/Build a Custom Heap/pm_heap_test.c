/*
 * @Author: Cao Ning
 * @Date: 2023-02-13 21:08:56
 * @FilePath: pm_heap_test.c
 * @Assignment: HW6 Build a Custom Heap
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include "pm_heap.h"

void test_pm_malloc_and_free()
{
    printf("The size of pm_heap is %d\n", PM_HEAP_SIZE);
    printf("The size of pm_heap after initialize is %d\n", pm_get_heap_free_size());
    printf("Each time we will allocate a BlockHeader and it will be cost size of %d\n\n", (int)sizeof(BlockHeader));

    printf("Testing pm_malloc and pm_free...\n");

    int *ptr1 = (int *)pm_malloc(sizeof(int));
    *ptr1 = 42;
    printf("Allocated int at %p, value = %d\n", ptr1, *ptr1);
    // printf("Size cost is %Current size of int = %d\n", pm_get_heap_free_size());
    printf("Size cost is %d + %d = %d byte, which is 0x%x. Current size is = %d\n\n", (int)sizeof(BlockHeader), (int)sizeof(int), (int)sizeof(BlockHeader) + (int)sizeof(int), (int)sizeof(BlockHeader) + (int)sizeof(int), pm_get_heap_free_size());

    char *ptr2 = (char *)pm_malloc(sizeof(char));
    *ptr2 = 'A';
    printf("Allocated char at %p, value = %c\n", ptr2, *ptr2);
    printf("Size cost is %d + %d = %d byte, which is 0x%x. Current size is = %d\n\n", (int)sizeof(BlockHeader), (int)sizeof(char), (int)sizeof(BlockHeader) + (int)sizeof(char), (int)sizeof(BlockHeader) + (int)sizeof(char), pm_get_heap_free_size());

    pm_free(ptr1);
    // extra one byte for '\0'
    char *ptr3 = (char *)pm_malloc(sizeof(char) * 11);
    for (int i = 0; i < 10; i++)
    {
        ptr3[i] = 'A' + i;
    }
    ptr3[10] = '\0';
    printf("Allocated char[10] at %p, value = %s\n", ptr3, ptr3);
    printf("Size cost is %d + %d = %d byte, which is 0x%x. Current size is = %d (11th is for end char)\n", (int)sizeof(BlockHeader), (int)sizeof(char) * 11, (int)sizeof(BlockHeader) + (int)sizeof(char) * 11, (int)sizeof(BlockHeader) + (int)sizeof(char) * 11, pm_get_heap_free_size());

    pm_free(ptr2);
    pm_free(ptr3);
    printf("\nCurrent size is %d byte after free ptr1, ptr2 and ptr3. It is equal to the size after initialize\n\n", pm_get_heap_free_size());

    // extra one byte for '\0'
    char *ptr4 = (char *)pm_malloc(sizeof(char) * 10 + 1);
    for (int i = 0; i < 10; i++)
    {
        ptr4[i] = 'A' + i;
    }
    ptr4[10] = '\0';
    printf("Allocated char[10] at %p, value = %s\n", ptr4, ptr4);
    printf("Size cost is %d + %d = %d byte, which is 0x%x. Current size is = %d (11th is for end char)\n", (int)sizeof(BlockHeader), (int)sizeof(char) * 11, (int)sizeof(BlockHeader) + (int)sizeof(char) * 11, (int)sizeof(BlockHeader) + (int)sizeof(char) * 11, pm_get_heap_free_size());

    printf("\npm_malloc and pm_free tests passed\n");
    printf("The address analysis is in the README\n");
}

int main()
{
    initialize_pm_heap();
    test_pm_malloc_and_free();
    return 0;
}