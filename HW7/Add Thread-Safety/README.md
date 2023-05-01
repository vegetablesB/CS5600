<!--
 * @Author: Cao Ning
 * @Date: 2023-02-26 13:46:01
 * @FilePath: README
 * @Assignment: HW7 Add Thread-Safety
 * @Course: CS5600 Spring 2023
-->

# Program-Managed Heap (pm-heap)

pm-heap is a simple memory allocation library for C programs. It provides the functions pm_malloc and pm_free for allocating and freeing memory from a single large chunk of memory.

# Design

I implement the code before with non-paged design, which means that the memory is not paged. And I changed to **paged design** first.

After that, I add the **thread-safety** to the pm-heap by using `pthread_mutex_t` to lock the access to the pm-heap data structures. And I use the `pthread_mutex_lock()` and `pthread_mutex_unlock()` functions to acquire and release the lock in the `pm_malloc()` and `pm_free()` functions.

# Implementation

In this implementation, I lock and unlock the mutex at the beginning and end of the `pm_malloc()` and `pm_free()` functions.

To avoid every thread allocating memory at the start of the pm-heap, I force the code have 40% chance to free the memory.

```c
if (rand() % 1000 > 600)
        {
            pm_free(ptr);
            printf("Thread %ld freed %zu bytes at %p\n", thread_id, size, ptr);
        }
```

With this modification, multiple threads can safely allocate and free memory from the pm-heap without interfering with each other.

# Testing

For paged design, I rewrite the test `pm_heap_test.c` to test the pm-heap. There are int, char, char list and char list which is bigger than the page size. And the test is passed. You can run the test by `make run1`.

For thread-safety, I write a test `pm_heap_test_thread.c` to test the pm-heap. As you can see, I create 4 threads and each thread will allocate and free memory from the pm-heap. And the test is passed. You can run the test by `make run` or `./pm_heap_test_thread`.

# Compilation

To compile the program, open a terminal and navigate to the directory containing the source files. Then, run the following command:

`make`

To clean the class files, run the following command:

`make clean`

To run the program for thread-safe test, execute the following command:

`make run`

or you can use `./pm_heap_test_thread` to run the program.

# Thanks

Thanks to TA taking time grading the homework. Thanks to Professor creating the homework.
