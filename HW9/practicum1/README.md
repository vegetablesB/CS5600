# Practicum I

This is a C library for managing a heap of persistent memory using a page table. The library uses a virtual memory address space, and pages are swapped in and out of memory to and from a disk file as needed. We are using LRU page replacement policy.

# Group Members

Ning Cao, Xiangyu Zeng

# Design

The persistent memory heap is implemented using a page table that maps virtual memory pages to physical memory pages and disk pages. The page table is an array of pm_page_table_entry structs, where each entry describes a page of memory, its size, its physical memory address, its disk offset, and other attributes.

The heap is implemented using a fixed-size array of char type, called pm_heap, which represents the entire virtual memory address space. Pages are allocated from this heap using the pm_malloc function, which finds a free page table entry and maps it to a free physical page in memory or on disk. Pages are swapped out of memory to disk when there are no more free physical pages available, and are swapped back into memory when they are needed again.

The pm_write and pm_read functions are used to write and read data to and from the allocated pages of memory. The pm_free function is used to free a previously allocated page of memory.

# Function Description

pm_init()
This function initializes the page table and opens the disk file for swapping pages to and from disk.

size_t pm_malloc(size_t size)
This function allocates a page of memory of the specified size. If there are no free physical pages available, it swaps out the least recently used page to disk. It returns the virtual address of the allocated page.

void pm_free(int virtual_address)
This function frees a previously allocated page of memory at the specified virtual_address. If the page is in memory, its contents are set to zero. If the page is in disk, a page of zeros is written to the disk file.

void *pm_write(int virtual_address, char*data, size_t size)
This function writes size bytes of data from the data buffer to the allocated page of memory at the specified virtual_address.

void *pm_read(int virtual_address)
This function reads the contents of the allocated page of memory at the specified virtual_address and returns a pointer to the buffer containing the contents.

void find_page_in_disk()
This function prints the virtual addresses of all pages that are currently in disk.

# Test Code Description

The pm_heap_test.c file contains a test program that spawns four threads, each of which allocates and writes data to pages of memory, reads some of the pages, and frees some of the pages. The program also calls the find_page_in_disk function to print the virtual addresses of all pages that are currently in disk.

# Compilation

To compile the program, open a terminal and navigate to the directory containing the source files. Then, run the following command:

`make`

To clean the class files, run the following command:

`make clean`

To run the program for thread-safe test, execute the following command:

`make run`

or you can use `./test` to run the program.

# Acknowledgments

I would like to thank my TA and professor for their guidance and support throughout the development of this project. Thanks for your effort creating this homework and taking time to grade it.
