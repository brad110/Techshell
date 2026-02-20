// Name(s): Bradley Duft
// Description: A project that implements a simple shell in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

//Constants
#define MAXINPUT 1024
#define MAXARGS 64

// Struct

//shell command holds everything parsed from users input
struct ShellCommand{
    char *command;          
    char *args[MAXARGS];    //args passed to execvp
    char *inputFile;        //file used for <
    char *outputFile;       //file used for >
};

/* 
    A function that causes the prompt to 
    display in the terminal
*/
void displayPrompt(){
    char cwd[PATH_MAX];
    //get cwd for prompt
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s$ ", cwd);
        fflush(stdout); //make sures prrompt prints
    } else{
        perror("getcwd() error");
    }
}

/*
    A function that takes input from the user.
    It may return return the input to the calling statement or 
    store it at some memory location using a pointer.
*/ 
char* getInput(){
    //allocate memory
    char *buffer = malloc(MAXINPUT);

    if (fgets(buffer, MAXINPUT, stdin) == NULL){
        //if user press ctrl+d, exits
        printf("\n");
        exit(0);
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

/*
    A function that parses through the user input.
*/
struct ShellCommand parseInput(char *input){
    struct ShellCommand cmd;
    cmd.command = NULL;
    cmd.inputFile = NULL;
    cmd.outputFile = NULL;

    int i = 0;

    //break input into tokens
    char *token = strtok(input, " ");

    while (token != NULL && i < MAXARGS - 1){
        //check for input redirect
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            cmd.inputFile = token;
        }
        //check for ouput redirect
        else if (strcmp(token, ">") == 0){
            token = strtok(NULL, " ");
            cmd.outputFile = token;
        }
        //if neither store as arg
        else {
            cmd.args[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    cmd.args[i] = NULL;

    if (i > 0)
        cmd.command = cmd.args[0];
    return cmd;
}


/*
    A function that executes the command. 
*/
void executeCommand(struct ShellCommand cmd){

    if (cmd.command == NULL)
        return;

    //Built in cd command to change working directory of shell
    if (strcmp(cmd.command, "cd") == 0){
        if (cmd.args[1] == NULL) {
            char *home = getenv("HOME");
            if (chdir(home) != 0)
                perror("cd error");
        } else {
            if (chdir(cmd.args[1]) != 0)
                perror("cd error");
        }
        return;
    }

    pid_t pid = fork();

    if (pid == 0){
        //CHILD PROCESS
        //Input redirection <
        if (cmd.inputFile != NULL) {
            FILE *fp = fopen(cmd.inputFile, "r");
            if (fp == NULL) {
                perror("input file error");
                exit(EXIT_FAILURE);
            }
            dup2(fileno(fp), STDIN_FILENO);
            fclose(fp);
        }

        //Output Redirection >
        if (cmd.outputFile != NULL) {
            FILE *fp = fopen(cmd.outputFile, "w");
            if (fp == NULL) {
                perror("output file error");
                exit(EXIT_FAILURE);
            }
            dup2(fileno(fp), STDOUT_FILENO);
            fclose(fp);
        }

        execvp(cmd.command, cmd.args);

        //If exec fails
        perror("exec error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0){
        //PARENT PROCESS
        wait(NULL);
    }
    else{
        perror("fork error");
    }
}


/*****************
        MAIN
 *****************/
int main() 
{
	char* input;
	struct ShellCommand command;
		
	// repeatedly prompt the user for input
	for (;;)
	{
        // display the prompt
        displayPrompt();

	    // get the user's input
	    input = getInput();
	    
	    // parse the command line
	    command = parseInput(input);
	    
	    // execute the command
	    executeCommand(command);

        //Helps to prevent memory leaks before next loop
        free(input);
	}

	exit(0);
}

