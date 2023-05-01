<!--
 * @Author: Cao Ning
 * @Date: 2023-02-14 05:54:36
 * @FilePath: README
 * @Assignment: HW6 Build a Custom Heap
 * @Course: CS5600 Spring 2023
-->
# Program-Managed Heap (pm-heap)

pm-heap is a simple memory allocation library for C programs. It provides the functions pm_malloc and pm_free for allocating and freeing memory from a single large chunk of memory.

# Design

pm-heap uses a linked list to keep track of the free blocks of memory in the heap. When pm_malloc is called, it iterates through the linked list to find a free block of memory with sufficient size. If it finds a suitable block, it marks the block as not free and returns a pointer to the memory immediately following the block header. If it doesn't find a suitable block, it returns a NULL pointer.

When pm_free is called, it takes a pointer to the memory that was previously returned by pm_malloc and marks the corresponding block as free. It then inserts the block into the linked list in the correct position based on the memory address of the block.

How the memory is allocated and freed is shown in the following diagram:

[24 bytes of struct] [The space allocated] [24 bytes of new block]

                    ↑(The pointer returned by pm_malloc is after the 24 bytes of struct)

# Implementation

pm-heap consists of three files: pm_heap.h, pm_heap.c, and pm_heap_test.c. pm_heap.h defines the BlockHeader structure, pm_malloc, and pm_free functions, as well as the size of the pm-heap. pm_heap.c contains the implementation of the functions, including the linked list manipulation. pm_heap_test.c contains a simple test function that tests pm_malloc and pm_free by allocating and freeing a couple of ints and chars.

# Testing

The following is the output of the test program:

The size of pm_heap is 10485760 //(10*1024*1024)

The size of pm_heap after initialize is 10485736 //(10*1024*1024 - 24)

Each time we will allocate a BlockHeader and it will be cost size of 24

Testing pm_malloc and pm_free...

Allocated int at 0xaaaae7732038, value = 42

Size cost is 24 + 4 = 28 byte, which is 0x1c. Current size is = 10485708 //(as you can see here, the size 10485708 = 10485736 - 28)

Allocated char at 0xaaaae7732054, value = A
Size cost is 24 + 1 = 25 byte, which is 0x19. Current size is = 10485683 //(as you can see here, the size 10485683 = 10485708 - 25, 0xaaae7732054 = 0xaaae7732038 + 0x1c)

// free the int at first block

Allocated char[10] at 0xaaaae773206d, value = ABCDEFGHIJ
Size cost is 24 + 10 = 34 byte, which is 0x22. Current size is = 10485676
Current size ois 10485701 byte after free ptr1 and ptr2 //(as you can see here, the first size(4 for the int) after free is not enough to allocate this 11 byte block because of fragmentation, so it will allocate after second block, which is 0xaaaae7732054 + 0x19 = 0xaaaae773206d)

// free char and the char[10], then the list is all free

Allocated char[10] at 0xaaaae7732038, value = ABCDEFGHIJ
Size cost is 24 + 10 = 34 byte, which is 0x22. Current size is = 10485666
pm_malloc and pm_free tests passed //(as you can see here, all the memory is allocated and freed successfully. The string starts from 0xaaaae7732038 again)

# Limitations

The current implementation does not handle fragmentation issues, so the pm-heap may become fragmented over time as blocks are allocated and freed. This could lead to performance degradation or the inability to allocate large blocks of memory even though there is enough free space in the pm-heap. In a real-world scenario, a fragmentation management strategy would need to be implemented.

# Compilation

To compile the program, open a terminal and navigate to the directory containing the source files. Then, run the following command:

make

To clean the class files, run the following command:

make clean

To run the program, execute the following command:

make run

or you can use ./pm_heap_test to run the program.

# Thanks

Thanks to TA taking time grading the homework. Thanks to Professor creating the homework.
