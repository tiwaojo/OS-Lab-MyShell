#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_CELLS 9
#define NUM_THREADS 27

#define BUFFER_LEN 256


typedef struct grid_data {
	int vertical;
	int horizontal;
	int used_table[NUM_CELLS];
} grid_data;

typedef struct row_data {
	int row;
	int used_table[NUM_CELLS];
} row_data;

typedef struct column_data {
	int column;
	int used_table[NUM_CELLS];
} column_data;


//The puzzle data read from the file
int puzzle_data[NUM_CELLS][NUM_CELLS];


void * check_grid(void *data) {
	int *res;
	grid_data *grid;
	int num;
	
	//Get the grid data passed in
	grid = (grid_data *) data;
	
	//Allocate space for the result
	res = (int *) calloc(1, sizeof(int));
	
	//Set the initial value as failure
	*res = 0;
	
	//Populate the used table with the entries in the puzzle
	for(int i = grid->vertical * 3; i < (grid->vertical * 3) + 3; i++) {
		for(int j = grid->horizontal * 3; j < (grid->horizontal * 3) + 3; j++) {
			num = puzzle_data[i][j];
			
			//Check if value not set
			if(num == 0) {
				continue;
			}
			
			//Check if number already appeared
			if(grid->used_table[num - 1] > 0) {
				
				//printf("%d already appeared in grid %d\n", num, (grid->vertical * 3) + grid->horizontal);
				
				//Release the memory used by our helper
				free(grid);
				
				//Return the error
				return res;
			}
			
			//Set the appearance flag of the number in our table
			grid->used_table[num - 1] = 1;
		}
	}
	
	//Set the value as success
	*res = 1;
	
	//Release the memory used by our helper
	free(grid);
	
	return res;
}

void *check_row(void *data) {
	int *res;
	row_data *row;
	int num;
	
	//Get the row data passed in
	row = (row_data *) data;
	
	//Allocate space for the result
	res = (int *) calloc(1, sizeof(int));
	
	//Set the initial value as failure
	*res = 0;
	
	//Populate the used table with the entries in the puzzle
	for(int i = 0; i < NUM_CELLS; i++) {
		num = puzzle_data[row->row][i];
		
		//Check if value not set
		if(num == 0) {
			continue;
		}
				
		//Check if number already appeared
		if(row->used_table[num - 1] > 0) {
			
			//printf("%d already appeared %d times in row %d\n", num, row->used_table[num - 1], row->row);
			
			//Release the memory used by our helper
			free(row);
			
			//Return the error
			return res;
		}
		
		//Set the appearance flag of the number in our table
		row->used_table[num - 1] = 1;
	}
	
	//Set the value as success
	*res = 1;
	
	//Release the memory used by our helper
	free(row);
	
	return res;
}

void *check_column(void *data) {
	int *res;
	column_data *col;
	int num;
	
	//Get the column data passed in
	col = (column_data *) data;
	
	//Allocate space for the result
	res = (int *) calloc(1, sizeof(int));
	
	//Set the initial value as failure
	*res = 0;	
	
	//Populate the used table with the entries in the puzzle
	for(int i = 0; i < NUM_CELLS; i++) {
		num = puzzle_data[i][col->column];
			
		//Check if value not set
		if(num == 0) {
			continue;
		}
		
		//Check if number already appeared
		if(col->used_table[num - 1] > 0) {
			
			//printf("%d already appeared %d times in column %d\n", num, col->used_table[num - 1], col->column);
			
			//Release the memory used by our helper
			free(col);
			
			//Return the error
			return res;
		}
		
		//Set the appearance flag of the number in our table
		col->used_table[num - 1] = 1;
	}
	
	//Set the value as success
	*res = 1;
	
	//Release the memory used by our helper
	free(col);
	
	return res;
}

//Reads the puzzle from 'puzzle.txt' and feeds it into the puzzle data
void read_puzzle() {
	FILE *fp;
	char buffer[BUFFER_LEN];
	int row, column;
	char *pos;
	
	//Open the puzzle file
	fp = /* WRITE YOUR OWN CODE HERE*/;
		
	//Check if open failed
	if(fp == NULL) {
		printf("ERROR: Unable to open puzzle.txt\n");
		exit(-1);
	}		
	
	//Clear the buffer
	memset(buffer, 0, BUFFER_LEN);
	
	//Set the initial row
	row = 0;
	
	//Get the numbers in the rows
	while(fgets(buffer, BUFFER_LEN, fp) != NULL) {
		pos = strtok(buffer, " ");
		
		//Check for overflow
		if(row >= 9) {
			printf("ERROR: Too many rows in puzzle\n");
			exit(-1);
		}
		
		//Set the initial column
		column = 0;
		
		//Get the numbers in the column
		while(pos != NULL) {
			
			//Check for overflow
			if(column >= 9) {
				printf("ERROR: Too many columns in puzzle\n");
				exit(-1);
			}
			
			//Check for non digit characters (not ascii 0 - 9)
			if((*pos < 48) || (*pos > 57)) {
				printf("ERROR: Invalid characters in puzzle\n");
				exit(-1);
			}
			
			//Set the number
			puzzle_data[row][column] = atoi(pos);
			
			//Check for invalid digits (like numbers > 9)
			if((puzzle_data[row][column] < 0) || (puzzle_data[row][column] > 9)) {
				printf("ERROR: Invalid digits in puzzle\n");
				exit(-1);
			}	
			
			//Get the next number
			pos = strtok(NULL, " ");
			
			column++;
		}
		
		//Check for mismatch
		if(column != 9) {
			printf("ERROR: Not enough columns in puzzle\n");
			exit(-1);
		}
		
		//Clear the buffer
		memset(buffer, 0, BUFFER_LEN);
		
		row++;
	}
	
	//Check for mismatch
	if(row != 9) {
		printf("ERROR: Not enough rows in puzzle\n");
		exit(-1);
	}
	
	//Close the file
	fclose(fp);
}

int main(void) {
	
	//Array holding the thread ids
	pthread_t thread_id[NUM_THREADS];
	
	//Holds the thread attributes
	pthread_attr_t attr;	
	
	//Pointer to our current grid helper data
	grid_data *grid_helper;
	
	//Pointer to our current row helper data
	row_data *row_helper;
	
	//Pointer to our current column helper data
	column_data *col_helper;
	
	//The result of the thread's calculations
	int **res;
	
	//The valid state of the puzzle
	int valid;
	
	
	//Read the puzzle data
	read_puzzle();
	
	//Initialize and set thread detached attribute, the attribute has been defined as attr
	/* WRITE YOUR OWN CODE HERE*/ 
	
	//Start the subgrid checking
	for(int i = 0; i < 3; i++) {		
		for(int j = 0; j < 3; j++) {
			
			//Allocate space for the grid data that will be passed to the thread
			grid_helper = (grid_data *) calloc(1, sizeof(grid_data));
			
			//Set the indices
			grid_helper->vertical = i;
			grid_helper->horizontal = j;
						
			//Create the thread and check if failed
			if(/* WRITE YOUR OWN CODE HERE*/) {
				printf("ERROR: Unable to create subgrid checking thread %d\n", i + 1);
				exit(-1);
			}		
			
			//The grid helper's memory will be cleared when the thread is finished using it
		}
	}
	
	//Start the row checking
	for(int i = 0; i < NUM_CELLS; i++) {		
			
		//Allocate space for the row data that will be passed to the thread
		row_helper = (row_data *) calloc(1, sizeof(row_data));
		
		//Set the index
		row_helper->row = i;
				
		//Create the thread and check if failed
		if(/* WRITE YOUR OWN CODE HERE*/) {
			printf("ERROR: Unable to create row checking thread %d\n", i + 1);
			exit(-1);
		}		
		
		//The row helper's memory will be cleared when the thread is finished using it
	}
	
	//Start the column checking
	for(int i = 0; i < NUM_CELLS; i++) {		
			
		//Allocate space for the column data that will be passed to the thread
		col_helper = /* WRITE YOUR OWN CODE HERE*/;
		
		//Set the index
		col_helper->column = i;
				
		//Create the thread and check if failed
		if(/* WRITE YOUR OWN CODE HERE*/) {
			printf("ERROR: Unable to create column checking thread %d\n", i + 1);
			exit(-1);
		}		
		
		//The column helper's memory will be cleared when the thread is finished using it
	}
	
	//Allocate space to store the resulting address
	res = (int **) calloc(1, sizeof(int *));
	
	//Set initial state to valid
	valid = 1;
	
	//Wait for the threads to terminate
	for(int i = 0; i < NUM_THREADS; i++) {
			
		//Try to do a thread join
		if(/* WRITE YOUR OWN CODE HERE*/) {
			printf("ERROR: Unable to join thread %d\n", i + 1);
			exit(-1);
		}
		
		//Check the result
		if(*(*res) != 1) {
			valid = 0;
		} 
	}
	
	//Check the final state
	if(valid == 1) {
		printf("valid\n");
	} else {
		printf("invalid\n");
	}
	
	//Cleanup
	free(res);
	pthread_attr_destroy(&attr);
	
	return 0;
}
