/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef BANKER_H_
#define BANKER_H_

#include <stdbool.h>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Data structure to implement the banker's algorithm
// Encode the state of the recourse-allocation system
typedef struct {
	// Available amount of each resource
	int available[NUM_RESOURCES];

	// Maximum demand of each customer
	int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

	// Amount currently allocated to each customer
	int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

	// Remaining need of each customer
	int need[NUM_CUSTOMERS][NUM_RESOURCES];
} BANK;

int customerData[NUM_RESOURCES] = { 0 };
 
/**
 * Request resources, returns true if successful
 * @param  n_customer index of the customer
 * @param  request    request by the customer
 * @return            boolean true, if the request has been accepted, false otherwise
 */
extern bool request_res(int n_customer, int request[]);

/**
 * Release resources, returns true if successful
 * @param  n_customer index of the customer
 * @param  release    to be released
 * @return            boolean true if the resource is released, false otherwise
 */
extern bool release_res(int n_customer, int release[]);

/**
 * Check if the system is in safe mode
 * @return true if the system is in safe mode, false otherwise
 */
extern bool isSafeSystem();

/**
 * Get the resource requests by the customer
 * @param  customerArg current customer
 * @return             NULL
 */
extern void *customerResourceRequest(void *customerArg);

/**
 * Display customer data
 * @param customer      current customer
 * @param customerData  data to be allocated
 * @param requestResult true if the request is accepted, false if denied
 * @param isRequest     true if the customer is requesting
 * @param isRelease     true if the customer is releasing
 */
extern void displayCustomerData(int customer, int customerData[], bool requestResult, bool isRequest, bool isRelease);

/**
 * Allocate the resources requested by the customer
 * @param  n_customer current customer
 * @param  request    requests by the customer
 * @return            true if the resource has been allocated, false otherwise
 */
extern bool allocateResources(int n_customer, int request[]);

#endif /* BANKER_H_ */
