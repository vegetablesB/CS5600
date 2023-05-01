/*
 * @Author: Cao Ning
 * @Date: 2023-01-21 14:22:57
 * @FilePath: queuetest.c
 * @Assignment:
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include "queue.h"

int main()
{
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    queue.size = 0;

    process_t p1 = {1, "Process 1", 2};
    process_t p2 = {2, "Process 2", 1};
    process_t p3 = {3, "Process 3", 3};
    process_t p4 = {4, "Process 4", 4};
    process_t p5 = {5, "Process 5", 5};

    printf("Adding Process 1, 2, 3, 4, 5 to queue\n");
    enqueue(&queue, &p1);
    enqueue(&queue, &p2);
    enqueue(&queue, &p3);
    enqueue(&queue, &p4);
    enqueue(&queue, &p5);

    printf("Queue size: %d\n", qsize(&queue));
    printf("Dequeued process: %s\n", ((process_t *)dequeue(&queue))->name);
    printf("Dequeued process (by priority): %s\n", dequeueProcess(&queue)->name);
    printf("Queue size: %d\n", qsize(&queue));

    return 0;
}
