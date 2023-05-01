/*
 * @Author: Cao Ning
 * @Date: 2023-01-21 14:01:51
 * @FilePath: queue.h
 * @Assignment: Implement Process Queue
 * @Course: CS5600 Spring 2023
 */
#ifndef QUEUE_H
#define QUEUE_H

typedef struct process {
    int id;
    char* name;
    int priority;
} process_t;

typedef struct node {
    void* data;
    struct node* next;
} node_t;

typedef struct queue {
    node_t* front;
    node_t* rear;
    int size;
} queue_t;

void enqueue(queue_t* queue, void* element);
void* dequeue(queue_t* queue);
process_t* dequeueProcess(queue_t* queue);
int qsize(queue_t* queue);
void printQueue(queue_t* queue);

#endif
