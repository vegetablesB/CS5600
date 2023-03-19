#include "pm_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#define NUM_THREADS 5

void *test_thread(void *arg)
{
    int thread_num = (int)(uintptr_t)arg;

    for (int i = 0; i < 10; i++)
    {
        size_t size = (rand() % 100 + 10) * sizeof(char);
        void *ptr = pm_malloc(size);
        printf("Thread %d allocated %zu bytes\n", thread_num, size);

        // Fill the allocated memory with random integers and keep a copy
        char *data = (char *)malloc(size);
        int num_ints = size / sizeof(char);
        for (int j = 0; j < num_ints; j++)
        {
            data[j] = 'A' + (rand() % 26);
            ((char *)ptr)[j] = data[j];
        }

        usleep(rand() % 100000); // Sleep between 0 and 100 ms

        // Print and verify the integers in the allocated memory
        // Print and verify the integers in the allocated memory
        printf("Thread %d memory content:", thread_num);
        for (int j = 0; j < num_ints; j++)
        {
            char c = pm_read_char((char *)ptr + j);
            // assert(data[j] == c);
            printf("%c", c);
        }
        printf("\n");

        free(data);
    }

    return NULL;
}

int main()
{
    srand(time(NULL));
    pm_init();

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, test_thread, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}