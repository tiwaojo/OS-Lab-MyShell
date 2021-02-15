#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "internals.h"

void add_command(char *name, int (*cmd_ptr)(int,char**))
{
	static int cmdi = 0;
	command_t *cmd = calloc(1, sizeof(command_t*));
	cmd->name = name;
	cmd->cmd_ptr = cmd_ptr;
	internal_commands[cmdi++] = cmd;
}

int internal_cmd_cd(int argc, char *argv[])
{
	// If more then one argument, change directory
	if (argc > 1)
	{
		// Change directory and return result
		
		/**********************************************/
		/*WRITE YOUR CODE HERE
		/**********************************************/
		int *result = chdir(argv[1]);
		if (result != 0) return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}

	// If no parameter, report current directory
	else
	{
		// Request current directory
		char *result = getcwd(0, 0);
		// Return error if result is still null
		if (result == NULL) return EXIT_FAILURE;
		// Print result
		printf("%s\n", result);
		return EXIT_SUCCESS;
	}
}

int internal_cmd_ls(int argc, char *argv[])
{
	DIR *dirptr;
	struct dirent *item;

	// Open current directory
	dirptr = opendir(".");

	// Return error if dp is dull
	if (dirptr == NULL) return 1;

	// Read and print filenames
	while (( item = readdir(dirptr) ))
	{
		/**********************************************/
		/*WRITE YOUR CODE HERE
		/**********************************************/
		printf("%s\n", item->d_name);
	}
	printf("\n");

	// Cleanup
	closedir(dirptr);
	return EXIT_SUCCESS;
}

int internal_cmd_environ(int argc, char *argv[])
{
	char **varstr = __environ;
	for (; *varstr != NULL; varstr++)
	{
		printf("%s\n", *varstr);
	}
	printf("\n");
	return 0;
}

int internal_cmd_echo(int argc, char *argv[])
{
	printf("%s", argv[1]);
	for (int i=1; i < argc; i++)
	{
		/**********************************************/
		/*WRITE YOUR CODE HERE
		/**********************************************/
		if (i != argc-1)
		{
			printf(" ");
		}
	}
	if (argc > 1) printf("\n");
	return 0;
}

int internal_cmd_help()
{
	char temp[255];

	FILE * fp;
	if ((fp = fopen("README.md", "r")) == NULL)
	{
		printf("README.md not found!");
		return 1;
	}

	printf("\n");

	while(fgets(temp, sizeof temp, fp) != NULL)
	{
		printf("%s", temp);

		if(feof(fp)) {
			fclose(fp);
			return 0;
		}
	}
}

int internal_cmd_quit()
{
	exit (0);
}

int internal_cmd_clr()
{
	printf("\e[1;1H\e[2J");
	return 0;
}

int internal_cmd_pause(){
	while(1){
		while(getchar()!='\n');
		return 0;
	}
}

void internals_add_all()
{
	internal_commands = calloc(50, sizeof(command_t*));
	//add command "echo" "cd" and "ls" 
	/**********************************************/
	/*WRITE YOUR CODE HERE
	/**********************************************/
	add_command("echo",   &internal_cmd_echo);
	add_command("cd",   &internal_cmd_cd);
	add_command("ls",   &internal_cmd_ls);
	add_command("environ",   &internal_cmd_environ);
	add_command("help", &internal_cmd_help);
	add_command("quit", &internal_cmd_quit);
	add_command("clr", &internal_cmd_clr);
	add_command("pause", &internal_cmd_pause);
}
