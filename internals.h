#ifndef INTERNALS_H_
#define INTERNALS_H_

// Define structure for internal commands
typedef struct {
	char *name; // name of command
	int (*cmd_ptr)(int,char**);
} command_t;

// Internal commands table
command_t **internal_commands;

void add_command(char *name, int (*cmd_ptr)(int,char**));
void internals_add_all();

#endif
