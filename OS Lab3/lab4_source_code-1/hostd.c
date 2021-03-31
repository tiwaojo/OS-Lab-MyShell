/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2021, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
#define MEMORY 1024

// Put global environment variables here

// Define functions declared in hostd.h here

/**
 * Starts or continues the first process in the queue, if possible.
 *
 * @return  1 if process could run, 0 otherwise
 */
int _start_or_continue(queue_t *queue, resources_t *available, memory_table_t *memory)
{
    node_t *n = queue_peek(queue);

    // Get process
    process_t *p = n->value;

    // Check if new or running process
    if (p->state == PROC_NEW)
    {
        // Check that resources can be allocated
        if ( ! (
               p->resources->printers <= available->printers
            && p->resources->scanners <= available->scanners
            && p->resources->modems   <= available->modems
            && p->resources->c_disks  <= available->c_disks
        ) )
        {
            return false; // Resources could not be allocated
        }

        // Allocate memory
        int result = alloc_mem(memory, p->mem_need);
        if (result == -1) return 0;

        p->mem_bloc = result;

        // Allocate resources
        available->printers -= p->resources->printers ;
        available->scanners -= p->resources->scanners ;
        available->modems   -= p->resources->modems   ;
        available->c_disks  -= p->resources->c_disks  ;

        printf("New process ------------------\n");
        printf("Priority:     %d\n", p->priority);
        printf("Mem Location: %d\n", p->mem_bloc);
        printf("Mem size:\n   %d\n", p->mem_need);
        printf("Resources:    %d %d %d %d\n" ,
            p->resources->printers,
            p->resources->scanners,
            p->resources->modems,
            p->resources->c_disks
        );

        pid_t pid = fork();
        if (pid == 0)
        {
            execlp("./process", "process");
            exit(0);
        }
        p->pid = pid;

    }
    else
    {
        // Process is already running - don't need to worry about resources
    }

    // Run process for 1 second
    kill(p->pid, SIGCONT);
    sleep(1);
    kill(p->pid, SIGSTOP);

    p->duration--;

}

void dispatch_cycle(queue_t *rt, queue_t *hi, queue_t *me, queue_t *lo,
    resources_t *available, memory_table_t *memory
) {

    if ( ! queue_is_empty(rt) )
    {
        if (_start_or_continue(rt, available, memory))
        {
            node_t *n = queue_peek(rt);
            // Get process
            process_t *p = n->value;

            // Pop if done
            if (p->duration == 0) {
                kill(p->pid, SIGINT);
                queue_pop(rt);
            }
            // Return since a process was executed
            return;
        }
    }

    if ( ! queue_is_empty(hi) )
    {
        if (_start_or_continue(rt, available, memory))
        {
            // Pop if done
            node_t *n = queue_pop(hi);
            // Get process
            process_t *p = n->value;

            if (p->duration == 0) {
                kill(p->pid, SIGINT);
            }
            else
            {
                queue_push(me, n->value);
            }
            // Return since a process was executed
        }
    }

}

int main(int argc, char *argv[])
{

    // Initialize state
    resources_t res_avail;  // available resources
    res_avail.printers = 2;
    res_avail.scanners = 1;
    res_avail.modems   = 1;
    res_avail.c_disks  = 2;

    // Initialize memory
    memory_table_t memory = create_memory_table(MEMORY);

    queue_t rt = create_queue();
    queue_t hi = create_queue();
    queue_t me = create_queue();
    queue_t lo = create_queue();

    char *dispatch_file;

    if (argc == 1)
    {
        printf("No arguments passed. Assuming default location(./dispatchlist)\n");
        dispatch_file = "dispatchlist";
    } 
    else
    {
        dispatch_file = argv[1];
    }
    // Load the dispatchlist
    pending_process_t *pending_processes = load_dispatchlist(dispatch_file);

    // Add each process structure instance to the job dispatch list queue

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
    int time_tick = 0;
    for (; 1; time_tick++)
    {
        int np = check_for_new_process(/*WRITE YOUR CODE HERE*/pending_processes,time_tick,&rt,&hi,&me,&lo);
        printf("Time: %03d > %d new processes have arrived\n", time_tick, np);
        dispatch_cycle(/*WRITE YOUR CODE HERE*/&rt,&hi,&me,&lo,&res_avail,&memory);
    }


    // Allocate the resources for each process before it's executed

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty
     
    return EXIT_SUCCESS;
}
