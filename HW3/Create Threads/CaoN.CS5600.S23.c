/*
 * @Author: Cao Ning
 * @Date: 2023-01-22 21:31:23
 * @FilePath: CaoN.CS5600.S23.c
 * @Assignment: Create Threads
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int global_data[10];
void *write_worker(void *data)
{
    for (int i = 0; i < 10; i++)
    {
        usleep(30000);
        global_data[i] += i * 10;
        printf("Thread ID = %li, name = %s, global_data[%d] = %d\n", (long)pthread_self(), (char *)data, i, global_data[i]);
    }
    return NULL;
}

void *read_worker(void *data)
{
    char *name = (char *)data;
    for (int i = 0; i < 10; i++)
    {
        usleep(50000);
        printf("Thread ID = %li, name = %s, global_data[%d] = %d\n", (long)pthread_self(), name, i, global_data[i]);
    }

    return NULL;
}

int main(void)
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, write_worker, (void *)"X");
    pthread_create(&th2, NULL, read_worker, (void *)"Y");
    sleep(1);
    printf("====> Cancelling Thread ID = %li, name = Y!!\n", (long)th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread ID = %li, name = X!!\n", (long)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}

/*
In my experiment, most of the time the result is consistent. Threads "X" and "Y", are incrementing a global data
simultaneously. I may not likely see in consistent results because write_worker() will more likely to finish
incrementing the global data before read_worker() can read it. This is because the read_worker() will sleep
for 50 microseconds before it can read the global data. However, the write_worker() will sleep for 30 microseconds
before it can increment the global data. This means that the write_worker() will finish incrementing the global data
before the read_worker() can read it. This is why I may not see inconsistent results.

However, as both threads are running at the same time, there is a potential for data inconsistency or
"stepping on each other." This is because the thread "X" may be incrementing the variable "count" while
the thread "Y" is also incrementing the same variable at the same time, leadingto unexpected or inconsistent
results.

To avoid this problem, a synchronization mechanism should be used to ensure that only one thread can
access the global array at a time. This would ensure that the producer thread is able to add data to
the array without the risk of the consumer thread reading it at the same time. Additionally, the use of
atomic operations or memory barriers can also be used to ensure consistency. Another way of avoiding the
problem is by adding a lock or semaphore to protect the global data array so that only one thread can
access it at a time. This will prevent other threads from reading or writing to the array while the first
thread is still using it.
 */
