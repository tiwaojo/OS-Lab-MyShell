/*
 * Lab 5 Banker's Algorithm
 *
 * Copyright (C) 2021
 * All rights reserved.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"
#include <unistd.h>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

BANK bank;
sem_t critical_section;

pthread_barrier_t customer_thread;

bool request_res(int n_customer, int request[])
{

    printf("\nAllocation Request from customer number %d\n", n_customer);
    printf("\nCustomer attepting to allocate: ");
    for (int i = 0; i < NUM_CUSTOMERS; i++)
        printf("%d ", request[i]);

    // Initiallise approval as true
    bool isApproved = true;

    // Loop through the number of resources for each customer and check if approved
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // check if request is greater than the need
        if (request[i] > bank.need[n_customer][i])
        {
            // Set the boolean to false, if it turns out to be greater
            isApproved = false;
            break;
        }
    }

    // If request is approved Execute the following
    if (isApproved)
    {
        sem_wait(&critical_section);

        // Loop through all resources
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            // check if request exceeds available resources
            if (request[i] > bank.available[i])
            {
                // do not approve the allocation of resources if request exceeds availability
                isApproved = false;
                break;
            }
        }

        isApproved = isApproved && allocateResources(n_customer, request);

        if (isApproved)
            printf("SYSTEM STATE SAFE, Proceed with allocation\n");
        else
            printf("\nSYSTEM STATE UNSAFE, Do not proceed\n");

        sem_post(&critical_section);
    }

    return isApproved;
}

bool release_res(int n_customer, int release[])
{
    printf("\nCustomer is trying to allocate: ");
    for (int i = 0; i < NUM_CUSTOMERS; i++)
        printf("%d ", release[i]);

    // Indicates if resource was released or not, true means was released.
    bool isApproved = true;

    // loop according to the number of resourced to be released
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // check if the amount currently allocated is less than the release request
        if (bank.allocation[n_customer][i] < release[i])
        {
            // Do not approve if release request is larger then allocated resources
            isApproved = false;
            break;
        }
    }

    // If the release of resources is approved execute the following
    if (isApproved)
    {
        sem_wait(&critical_section);

        // loop through resources
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            // empty the allocated resource
            bank.allocation[n_customer][i] -= release[i];

            // empty the remaining need for the customer
            bank.need[n_customer][i] += release[i];

            // increment available resource for every resource released
            bank.available[i] += release[i];
        }

        sem_post(&critical_section);
    }

    return isApproved;
}

bool allocateResources(int n_customer, int request[])
{
    // Initialize arrays to store resources current information
    int available[NUM_RESOURCES];
    int allocation[NUM_RESOURCES];
    int need[NUM_RESOURCES];

    // loop through all resources
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // Assign available resources to the arrays
        available[i] = bank.available[i];
        allocation[i] = bank.allocation[n_customer][i];
        need[i] = bank.need[n_customer][i];
    }

    // loop through all resources
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // Increment the amount of resources allocated by the request
        bank.allocation[n_customer][i] += request[i];

        // Decrement need of each customer by the request
        bank.need[n_customer][i] -= request[i];

        // Decrement resource availability by the request
        bank.available[i] -= request[i];
    }

    // Check if the system is safe
    if (isSafeSystem())
        // if safe return true
        return true;
    else
    {
        //Loop through all resources
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            // allocate resources based on the banker algorithm data structure
            bank.available[i] = available[i];
            bank.allocation[n_customer][i] = allocation[i];
            bank.need[n_customer][i] = need[i];
        }

        // indicated failure of allocation by returning false
        return false;
    }
    return false;
}

bool isSafeSystem()
{
    // Array storing resource's data
    int res[NUM_RESOURCES];

    // Array if in finished state
    bool isFinished[NUM_RESOURCES];

    // loop through all resources
    for (int i = 0; i < NUM_RESOURCES; i++)
        // set the element to false
        res[i] = false;

    // copy the available data to the res
    memcpy(res, bank.available, sizeof(res));

    // loop through the number of customers
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        // Check if the the state of finish is false
        if (!isFinished[i])
        {
            // loop through the number of resource
            for (int j = 0; j < NUM_RESOURCES; j++)
            {
                // check if the bank remaining need
                if (bank.need[i][j] <= res[j])
                {
                    // loop through the number of resources
                    for (int x = 0; x < NUM_RESOURCES; x++)
                        // set the allocation data to the array of resource
                        res[x] += bank.allocation[i][j];

                    // indicate the state of finish to true
                    isFinished[i] = true;
                }
            }
        }
    }

    // loop through the number of resources
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // if the allocation isn't finished, return false
        if (!isFinished[i])
            return false;
    }

    // return true if the system is safe
    return true;
}

/**
 * Get the resource requests by the customer
 * @param  customerArg current customer
 * @return             NULL
 */
void *customerResourceRequest(void *customerArg)
{
    // Get the customer
    int customer = *(int *)customerArg;

    // Synchronize participating threads at the barrier
    pthread_barrier_wait(&customer_thread);

    // Continually loop
    while (true)
    {
        // State of the request check
        bool checkForRequest = true;

        // Loop through the number of resources
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            // Random resource request
            customerData[i] = rand() % (bank.maximum[customer][i] + 1);

            // Check for empty resource request
            if (customerData[i] != 0)
                checkForRequest = false;
        }

        // If the request is false
        if (!checkForRequest)
        {
            // print the customer information on the console
            displayCustomerData(customer, customerData, request_res(customer, customerData), true, false);
            sleep(2);
        }

        // State of the release check
        bool checkForRelease = true;

        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            // Release customer resource
            customerData[i] = rand() % (bank.allocation[customer][i] + 1);

            // Check if the release state returned 0
            if (checkForRelease != 0)
                checkForRelease = false;
        }

        // Check if release state is true
        if (!checkForRelease)
        {
            displayCustomerData(customer, customerData, request_res(customer, customerData), false, true);
            sleep(2);
        }
    }

    return NULL;
}

/**
 * Display customer data
 * @param customer      current customer
 * @param customerData  data to be allocated
 * @param requestResult true if the request is accepted, false if denied
 * @param isRequest     true if the customer is requesting
 * @param isRelease     true if the customer is releasing
 */
void displayCustomerData(int customer, int customerData[], bool requestResult, bool isRequest, bool isRelease)
{
    // decrement lock the semaphore pointed to by sem
    sem_wait(&critical_section);

    // display the remaining needs for the customer
    printf("\nRemaining Need for customer %d:\n", customer);
    for (int i = 0; i < NUM_RESOURCES; i++)
        printf("%d ", bank.need[customer][i]);

    // determine wether customer is requesting or releasing resources
    if (isRequest)
        printf("\n Attempting to Request: ");
    else if (isRelease)
        printf("\n Attempting to Release: ");

    // loop through the number of resources being requested or released
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        // print out data to console
        printf("%d ", customerData[i]);
    }

    // display if the request was approved or denied
    if (requestResult)
        printf("\n Requested Resource State was ACCEPTED\n");
    else
        printf("\n Requested Resource State was DENIED\n");

    // display the available resources
    printf("\n Resource Available: ");
    for (int i = 0; i < NUM_RESOURCES; i++)
        printf("%d ", bank.available[i]);

    printf("\n");

    sem_post(&critical_section);
}

int main(int argc, char *argv[])
{
    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments
    if (argc != NUM_RESOURCES + 1)
    {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }
    else
        // Loop through the number of resources
        for (int i = 0; i < NUM_RESOURCES; i++)
            // Add the resources to the bank
            // Allocate the available resources
            bank.available[i] = atoi(argv[i + 1]);

    // Initialize the locks, mutexes
    /* WRITE YOUR OWN CODE HERE  HINT: use "sem_init(**,**,**)" */
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL); //create mutex lock

    pthread_mutex_lock(&mutex); //acquire mutex lock

    sem_t sem;

    sem_init(&sem, 0, 1); //create semaphore & initialize to 1

    // Loop through the number of customers
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        // Loop through the number of resources for each customers
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            // Initialize random resource
            int randomResource = (rand() % (bank.available[j] - 1)) + 1;

            // Add it to the maximum demand of the customer
            bank.maximum[i][j] = randomResource;

            // Set the allocation array to 0
            bank.allocation[i][j] = 0;

            bank.need[i][j] = bank.maximum[i][j];
        }
    }

    // Initialize pthread
    /* WRITE YOUR OWN CODE HERE */
    // pthread_barrier_t

    // Initialize pthread array of NUM_CUSTOMERS
    /* WRITE YOUR OWN CODE HERE */
    pthread_t ids[NUM_CUSTOMERS];

    // Loop through the number of customers to create pthreads
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        int *customer = malloc(sizeof(int));
        *customer = i;

        // Create pthread
        /* WRITE YOUR OWN CODE HERE */
        pthread_create(&ids[i], NULL, customerResourceRequest, customer);

        // wait for the thread to process
        /* WRITE YOUR OWN CODE HERE */
        sem_wait(&sem); //Acquire the semaphore
    }
    sem_post(&sem); //release the semaphore
    // Join Customer Threads
    for (int i = 0; i < NUM_CUSTOMERS; i++)
        /* WRITE YOUR OWN CODE HERE */
        pthread_join(ids[i], NULL);

    return EXIT_SUCCESS;
}
