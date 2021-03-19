/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2021, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

queue_t create_queue()
{
    queue_t queue;

    // Head and tail will both be the same NULL node
    // , since the queue is empty
    queue.head = calloc(1, sizeof(node_t));
    queue.tail = queue.head;

    return queue;
}

node_t *queue_push(queue_t *queue, void *value)
{
    // Create node with correct values
    node_t *node = calloc(1,sizeof(node_t));
    node->value = value;
    node->next = NULL;

    // Set last node's next node
    queue->tail->next = node;

    // Set tail to this node
    queue->tail = node;

    return node;
}

node_t *queue_pop(queue_t *queue)
{
    node_t *node = queue->head->next;

    if (node == NULL)
    {
        return NULL;
    }

    queue->head->next = node->next;

    return node;
}

node_t *queue_peek(queue_t *queue)
{
    node_t *node = queue->head->next;

    if (node == NULL)
    {
        return NULL;
    }

    return node;
}

int queue_is_empty(queue_t *queue)
{
    node_t *n = queue_peek(queue);
    return (n == NULL) ? 1 : 0;
}
