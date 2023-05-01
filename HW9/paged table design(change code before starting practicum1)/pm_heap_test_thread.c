#include "pm_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_ALLOCATIONS 10
#define MAX_ALLOCATION_SIZE 8000

void *thread_func(void *arg)
{
    long thread_id = (long)arg;
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        // Allocate a random size block
        size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
        usleep(rand() % 1000);
        void *ptr = pm_malloc(size);
        if (ptr == NULL)
        {
            printf("Thread %ld failed to allocate %zu bytes\n", thread_id, size);
            pthread_exit(NULL);
        }
        printf("Thread %ld allocated %zu bytes at %p\n", thread_id, size, ptr);

        // Free the block with a 40% chance
        if (rand() % 1000 > 600)
        {
            pm_free(ptr);
            printf("Thread %ld freed %zu bytes at %p\n", thread_id, size, ptr);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    pm_init();

    pthread_t threads[NUM_THREADS];
    for (long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed successfully\n");

    return 0;
}
