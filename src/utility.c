/* 
Name: Adam McKeown
Student Number: 22462626
I, Adam McKeown, acknowledge all of DCU's Academic Integrity Policies. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include "commands.h"
#include <fcntl.h>

#define MAX_BUFFER 1024
#define SEPARATORS " \t\n"

time_t getTime() {
    return time(NULL);
}

void welcome() {
    // Display welcome message with information and system time
    printf("+---------------------------------------+\n");
    printf("|                                       |\n");
    printf("|                MyShell                |\n");
    printf("|                                       |\n");
    printf("|          Name: Adam Mckeown           |\n");
    printf("|       Student Number: 22462626        |\n");
    printf("|                                       |\n");
    printf("+---------------------------------------+\n");

    char* username = getenv("USER");

 // https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
    time_t t = getTime();
    struct tm *tm_info = localtime(&t);
    printf("\n\nSystem time:    %02d:%02d:%02d", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("\nWelcome,        @%s", username);
    printf("\nUse \"help\" to access the shell manual.");
    printf("\n\n");

    sleep(1); // Pause for readability
}

void prompt() {
    // Display the command prompt with username, system name, time, and current directory
    char cwd[MAX_BUFFER];
    char* name = getenv("NAME");
    char* username = getenv("USER");
    time_t t = getTime();
    struct tm *tm_info = localtime(&t);
    getcwd(cwd, sizeof(cwd));
    printf("\n╭── %s@%s %02d:%02d:%02d \n└── <%s> ", username, name, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, cwd);
}

char **splitline(char *input) {
    // Split input string into tokens based on whitespace separators
    char *token;
    char **tokens = malloc(MAX_BUFFER * sizeof(char *));
    int i = 0;

    if (!tokens) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, SEPARATORS);

    while (token != NULL) {
        tokens[i++] = token;

        if (i >= MAX_BUFFER) {
            perror("Buffer overflow");
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, SEPARATORS);
    }

    tokens[i] = NULL;
    return tokens;
}

int hasampersand(char *args[])
{
    // Check if the last argument is "&", indicating background execution
    int count = 0;
    for (int i = 1; args[i] != NULL; ++i) {
        ++count;
    }
    if (!strcmp(args[count], "&")) {
    	return true;
    } else {
        return false;
    }
}

#include <sys/stat.h>
#include <fcntl.h>

int backgroundexecute(char **args) {
    // Execute commands in the background
    pid_t pid = fork(); // creates a child process
    if (pid == 0) {
        // Redirect standard output to /dev/null
        int dev_null_fd = open("/dev/null", O_WRONLY);
        if (dev_null_fd == -1) {
            perror("Error opening /dev/null");
            exit(EXIT_FAILURE);
        }
        dup2(dev_null_fd, STDOUT_FILENO);
        close(dev_null_fd);

        if (!strcmp(args[0], "cd")) {
            if (cd(args[1]) != 0) {
                perror("cd command failed");
            }
        } else if (!strcmp(args[0], "echo")) {
            echo(args[1]);
        } else if (!strcmp(args[0], "clr")) {
            // clear terminal command
            clr();
        } else if (!strcmp(args[0], "dir")) {
            // list directory command
            dir(args);
        } else if (!strcmp(args[0], "environ")) {
            // get environment command
            list_environment();
        } else if (!strcmp(args[0], "help")) {
            // print readme file
            help();
        } else if (!strcmp(args[0], "pause")) {
            // pause shell command
            pause();
        } else if (!strcmp(args[0], "quit")) {
            // quit shell command
            quit();
        } else {
            // if the command does not exist then print the following
            printf("\nError, command \"%s\" not found. Running command in system shell.\n", args[0]);
        }
        exit(EXIT_SUCCESS); // Ensure child process exits after command execution
    } else if (pid == -1) {
        perror("Error, fork failed.\n");
    }
    return 0;
}