/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef QUEUE_H_
#define QUEUE_H_

// Your linked list structure for your queue
// typedef ... 
//  ...
//  proc process;
//  ...
//} node_t; 
typedef struct node_t_
{
	void *value;
	struct node_t_ *next;
} node_t;

typedef struct queue_t_
{
	node_t *head;
	node_t *tail;
} queue_t;

// Include your relevant FIFO queue functions declarations here they must start 
// with the extern keyword such as in the following examples:

// Add a new process to the queue, returns the address of the next node added
// to the linked list

extern queue_t create_queue(void);

extern node_t *queue_push(queue_t *queue, void *value);

extern node_t *queue_pop(queue_t *queue);

extern node_t *queue_peek(queue_t *queue);

extern int queue_is_empty(queue_t *queue);


#endif /* QUEUE_H_ */