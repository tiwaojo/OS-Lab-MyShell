/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

// Define your utility functions here, you will likely need to add more...

memory_table_t create_memory_table(int size)
{
    memory_table_t memory;

    // Create head node
    memory.free_memory = calloc(1, sizeof(node_t));

    // Create first entry of free memory
    memory_table_entry_t *first_entry = calloc(1, sizeof(memory_table_entry_t));
    first_entry->location = 0;
    first_entry->size = size;

    // Add first entry to linked list
    node_t *node = calloc(1, sizeof(node_t));
    node->value = first_entry;
    memory.free_memory->next = node;

    return memory;
}

int alloc_mem(memory_table_t *memory, int size)
{
	node_t *node = memory->free_memory; // free memory node
	node_t *last_node;
	while (1)
	{
		// Get next node
		last_node = node;
		node = node->next;

		// Break if no next node
		if (node == NULL) break;

		// Check if this memory is large enough
		memory_table_entry_t *entry = node->value;
		if (entry->size >= size)
		{
			// Get location
			int loc = entry->location;

			// Resize or remove available chunk
			entry->size = entry->size - size;
			if (entry->size == 0)
			{
				// Eliminate this memory chunk
				last_node->next = node->next;
			}
			else
			{
				// Reposition memory chunk
				entry->location = loc + size;
			}

			// Return location of available memory
			return loc;
		}
	}

	return -1;
}

void _de_partition_mem(memory_table_t *memory, int *loc, int *size)
{
	node_t *n = memory->free_memory; // node containing free memory entry
	node_t *last_node;
	memory_table_entry_t *entry;     // free memory entry

	// printf("Departition with: %d,%d\n", *loc, *size);

	while (1)
	{
		last_node = n;
		if ((n = n->next) == NULL)
			break;

		entry = (memory_table_entry_t*)(n->value);

		// Adjacent free memory to the left
		if (entry->location + entry->size == *loc)
		{
			// printf("Expanding into %d\n", entry->location);
			// Expand into left memory
			*loc = entry->location;
			*size += entry->size;
			// Remove old memory information
			last_node->next = n->next;
			// Recursively check for adjacent chunks of free memory
			_de_partition_mem(memory,loc,size);
			return;
		}
		// Adjacent free memory to the right
		else if (entry->location == *loc + *size)
		{
			// printf("Expanding into %d\n", entry->location);
			// Expand into right memory
			*size += entry->size;
			// Remove old memory information
			last_node->next = n->next;
			// Recursively check for adjacent chunks of free memory
			_de_partition_mem(memory,loc,size);
			return;
		}
	}
}

int free_mem(memory_table_t *memory, int loc, int size)
{
	// First, we much check for adjecent free memory to avoid partitioning

	// Expand into adjacent partitions of free memory
	_de_partition_mem(memory,&loc,&size);

	node_t *n = memory->free_memory; // node containing free memory entry
	memory_table_entry_t *entry;     // free memory entry

	// while ((n = n->next) != NULL)
	// {
	// 	entry = (memory_table_entry_t*)(n->value);

	// 	// Adjacent free memory to the left
	// 	if (entry->location + entry->size == loc)
	// 	{
	// 		// Expand left memory
	// 		entry->size += size;
	// 		return 0; // nothing left to do
	// 	}
	// 	// Adjacent free memory to the right
	// 	else if (entry->location == loc + size)
	// 	{
	// 		// Expand right memory
	// 		entry->location = loc;
	// 		entry->size += size;
	// 		return 0; // nothing left to do
	// 	}
	// }

	// Create new free memory entry
	entry = calloc(1, sizeof(memory_table_entry_t));
	entry->location = loc;
	entry->size = size;

	// Create node for free memory entry
	n = calloc(1, sizeof(node_t));
	n->value = entry;

	// Add node to free memory linked list
	node_t *old_next = memory->free_memory->next;
	n->next = old_next;
	memory->free_memory->next = n;


	return 0;
}

// Function to 
pending_process_t *load_dispatchlist(char *dispatch_file)
{
     FILE *file;
     file = fopen(dispatch_file, "r");
     if (file == NULL) exit(EXIT_FAILURE);

     char *buffer = NULL;
     size_t line_size = 0;
     int num_lines = 1;

     while(1)
     {
         getline(&buffer, &line_size, file);
         num_lines++;
         if(feof(file)) break;
     }
     
     fclose(file);
     
     pending_process_t *pending_processes = calloc(num_lines, sizeof(pending_process_t));

     int num_process = 0;
     file = fopen(dispatch_file, "r");

     while(!feof(file))
     {
         getline(&buffer, &line_size, file);
         if(buffer[0] == '\n') continue;

         pending_process_t *temp_process = &pending_processes[num_process];

         char *token;
         for (int i=0; token = erics_lazy_tokenize(&buffer); i++)
         {
             if (token == NULL) break;

             switch(i)
             {
                 case 0: temp_process->arrival_time = atoi(token); break;
                 case 1: temp_process->priority = atoi(token); break;
                 case 2: temp_process->proc_time = atoi(token); break;
                 case 3: temp_process->memory_needed = atoi(token); break;
                 
                 case 4: temp_process->n_printers = atoi(token); break;
                 case 5: temp_process->n_scanners = atoi(token); break;
                 case 6: temp_process->n_modems = atoi(token); break;
                 case 7: temp_process->n_c_disks = atoi(token); break;
             }
         }
         num_process++;
     }
	 pending_process_t *dispatch_terminator = &pending_processes[num_process];
	 dispatch_terminator->arrival_time = -1;

     return pending_processes;
}

// Really lazy tokenizer.
// Assumes everything below 0x30 is a delimiter.
char *erics_lazy_tokenize(char **buffer)
{
    char *token = calloc(256, 1);

    // Return if empty
    if (*buffer[0] == '\0' || *buffer[0] == '\n') return NULL;

    // Copy the first item in the buffer
    strncpy(token, *buffer, 255);

    /* Magical block of code that makes it work */
    {
        char *marker;
        for (marker = token; *marker >= 0x30; marker++);
        *marker = '\0';
        for (marker++; *marker < 0x30 && *marker != 0; marker++);
        *buffer = marker;
    }

    return token;
}

int check_for_new_process(
	// List of pending processes and time slice
	pending_process_t *list, int time_slice,

	// Queues which processes can be placed on
	queue_t *q_realtime,
	queue_t *q_user_hi,
	queue_t *q_user_me,
	queue_t *q_user_lo
) {
	int amount = 0;

	// Pointer to current process being looked at
	pending_process_t *proc = list;

	// Loop through list of pending processes
	for (; proc->arrival_time != -1; proc++) // -1 arrival time is our terminator
	{
		// Check if process should arrive now
		if (proc->arrival_time == time_slice)
		{
			amount++; // amount of processes added

			// Create resources_t
			resources_t *resources = calloc(1, sizeof(resources_t));
			resources->printers = proc->n_printers ;
			resources->scanners = proc->n_scanners ;
			resources->modems   = proc->n_modems   ;
			resources->c_disks  = proc->n_c_disks  ;

			// Create process_t
			process_t *process = calloc(1, sizeof(process_t));
			process->priority  = proc->priority;
			process->duration  = proc->proc_time;
			process->mem_bloc  = -1;         // -1 means not allocated yet
			process->mem_need  = proc->memory_needed;
			process->resources = resources;
			process->state     = PROC_NEW;   // resources to be allocated

			// Add to appropriate queue
			switch(proc->priority)
			{
				case PRIORITY_RT:
					queue_push(q_realtime, process);
					break;
				case PRIORITY_1:
					queue_push(q_user_hi, process);
					break;
				case PRIORITY_2:
					queue_push(q_user_me, process);
					break;
				case PRIORITY_3:
					queue_push(q_user_lo, process);
					break;
			}
		}
	}

	return amount;
}