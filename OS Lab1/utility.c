/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2021, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"

// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

/**
 * Print the current working directory to stdout
 *
 * This function returns the current directory. Error
 * is handled silently. No linefeed is printed.
 */

void util_work_dir(){
	//*util_print_cwd() probably does the same thing. Just didn't realize it and kinda don't wat do delete it yet
	char *buf;
	if(getcwd(buf,sizeof(buf)) !=NULL){
		printf("Current working directory: %s\n",buf);
	}else{
		exit(0);
	}
}

char *util_print_cwd()
{
	// Request current directory
	char *result = getcwd(0, 0);
	// Return if result is still null
	if (result == NULL) return "";
	
	return result;
}
