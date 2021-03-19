/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef UTILITY_H_
#define UTILITY_H_

#include "queue.h" // Need definitions for queue types

// The amount of available memory
#define MEMORY 1024 // total bytes of memory
#define BITS64 MEMORY/64   // number of 64MB portions

#include "queue.h"

typedef struct memory_table_entry_t_
{
	int location;
	int size;
} memory_table_entry_t;

typedef struct memory_table_t_
{
	// Linked-list of free memory locations
	node_t *free_memory; // node points to a memory_table_entry_t
} memory_table_t;

typedef enum proc_state_ {PROC_NEW, PROC_PAUSED, PROC_RUNNING} proc_state_t;

typedef enum priority_t_ {
	PRIORITY_RT=0,
	PRIORITY_1=1,
	PRIORITY_2=2,
	PRIORITY_3=3
} priority_t;

// Resources structure containing integers for each resource constraint and an
// array of 1024 for the memory
typedef struct resources_t_
{
	int printers;
	int scanners;
	int modems;
	int c_disks;
} resources_t;


// Processes structure containing all of the process details parsed from the 
// input file, should also include the memory address (an index) which indicates
// where in the resources memory array its memory was allocated
typedef struct process_t_
{
	int           priority;
	int           duration;
	int           mem_bloc;
	int           mem_need;
	int           pid;
	resources_t  *resources;
	proc_state_t  state;
	
} process_t;

typedef struct pending_process_t_
{
	unsigned char already_added; // Set this to 1 after adding to a queue

	// Information provided by the dispatch list
	int arrival_time;  // time slice to arrive
	int priority;      // starting priority
	int proc_time;     // time required for process to complete
	int memory_needed; // amount of memory needed by process in bytes

	// Resources needed by process
	int n_printers;    // number of printers
	int n_scanners;    // number of scanners
	int n_modems;      // number of modems
	int n_c_disks;     // number of compact disks
} pending_process_t;



// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following examples:

extern memory_table_t create_memory_table(int size);

// Function to allocate a contiguous chunk of memory in your resources structure
// memory array, always make sure you leave the last 64 values (64 MB) free, should
// return the index where the memory was allocated at

/**
 * Allocate memory for a process
 *
 * @param memory     table of memory available to processes
 * @param size       amount of memory to allocate in bytes
 * @return           location where memory is allocated
 */
extern int alloc_mem(memory_table_t *memory, int size);

/**
 * @param available  resources available on the system
 * @param needed     resources needed by the process
 * @return           0 on success, 1 if resources unavailable
 */
extern int alloc_res(resources_t *available, resources_t needed);

// Function to free the allocated contiguous chunk of memory in your resources
// structure memory array, should take the resource struct, start index, and 
// size (amount of memory allocated) as arguments
// extern free_mem(resources res, int index, int size);
/**
 * Free memory for a process
 *
 * @param memory     table of memory available to processes
 * @param loc        location of memory to free
 * @param size       amount of memory to free
 * @return           0 on success, 1 on failure
 */
extern int free_mem(memory_table_t *memory, int loc, int size);

// Function to parse the file and initialize each process structure and add
// it to your job dispatch list queue (linked list)
// extern void load_dispatch(char *dispatch_file, node_t *queue);

/**
 * Loads process information from dispatchlist to memory
 *
 * @param dispatch_file  name of file to load from
 * @return  pointer to pending_process_t entries with null terminator
 */
extern pending_process_t *load_dispatchlist(char *dispatch_file);

/**
 * Places new processes on the queue
 *
 * Checks each pending process' arrival time against the current time slice.
 * If an arrival time matches, a proccess_t struct is created from the matching
 * pending_process_t, which is then added to the correct queue according to
 * the priority value.
 *
 * @param list        list of pending processes
 * @param time_slice  current time slice in seconds
 *
 * @param q_realtime  realtime process queue
 * @param q_user_hi   user job queue with priority 1 tasks
 * @param q_user_me   user job queue with priority 2 tasks
 * @param q_user_lo   user job queue with priority 3 tasks
 * 
 */
extern int check_for_new_process(

	// List of pending processes and time slice
	pending_process_t *list, int time_slice,

	// Queues which processes can be placed on
	queue_t *q_realtime,
	queue_t *q_user_hi,
	queue_t *q_user_me,
	queue_t *q_user_lo

);

char *erics_lazy_tokenize(char **buffer);

#endif /* UTILITY_H_ */