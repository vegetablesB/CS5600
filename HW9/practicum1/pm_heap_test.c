#include "pm_heap.h"
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 5

void *thread_func(void *arg)
{
    int thread_num = (int)(uintptr_t)arg;
    for (int i = 0; i < NUM_ITERATIONS - 1; i++)
    {
        size_t size = (rand() % 10 + 5) * sizeof(char);
        int v_addr = pm_malloc(PM_PAGE_SIZE);

        // Generate a random string of length 'size'
        char data[size + 1];
        for (int j = 0; j < (int)size; j++)
        {
            data[j] = 'A' + rand() % 26;
        }
        data[size] = '\0';

        // Write and read the data
        printf("\nThread %d:", thread_num);
        pm_write(v_addr, data, size + 1);
        usleep(10000);

        if (rand() % 1000 > 600)
        {
            printf("\n_____Thread %d:", thread_num);
            pm_read(v_addr);
        }

        // assert(strcmp(buffer, data) == 0);
        if (rand() % 1000 > 800)
        {
            printf("\nThread %d: Freeing virtual address %d\n", thread_num, v_addr);
            pm_free(v_addr);
        }
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
        pthread_create(&threads[i], NULL, thread_func, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    find_page_in_disk();
    return 0;
}