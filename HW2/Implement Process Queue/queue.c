/*
 * @Author: Cao Ning
 * @Date: 2023-01-21 14:01:45
 * @FilePath: queue.c
 * @Assignment: Implement Process Queue
 * @Course: CS5600 Spring 2023
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @description: Enqueue a process to the queue
 * @param {queue_t*} queue
 * @param {process_t*} process
 * @return {*}
 */
void enqueue(queue_t* queue, void* element) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = element;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
        queue->rear = new_node;
    } else {
        queue->front = new_node;
        queue->rear = new_node;
    }
    queue->size++;
}

/**
 * @description: Dequeue a process from the queue
 * @param {queue_t*} queue
 * @return {*}
 */
void* dequeue(queue_t* queue) {
    // if the queue is empty, return NULL
    if (!queue->front)
        return NULL;

    process_t* data = (process_t*)queue->front->data;
    node_t* temp = queue->front;
    queue->front = queue->front->next;
    if (!queue->front)
        queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

/**
 * @description: Dequeue a process from the queue with the highest priority
 * @param {queue_t*} queue
 * @return {*}
 */
process_t* dequeueProcess(queue_t* queue) {
    if (!queue->front)
        return NULL;
    process_t* data = (process_t*)queue->front->data;
    node_t* temp = queue->front;
    node_t* prev = NULL;
    node_t* curr = queue->front;
    while (curr) {
        process_t* curr_data = (process_t*)curr->data;
        if (curr_data->priority > data->priority) {
            data = curr_data;
            temp = curr;
            prev = curr;
        }
        curr = curr->next;
    }
    if (prev) {
        prev->next = temp->next;
    } else {
        queue->front = temp->next;
    }
    if (!queue->front)
        queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

/**
 * @description: Get the size of the queue
 * @param {queue_t*} queue
 * @return {*}
 */
int qsize(queue_t* queue) {
    return queue->size;
}
