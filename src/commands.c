/* 
Name: Adam McKeown
Student Number: 22462626
I, Adam McKeown, acknowledge all of DCU's Academic Integrity Policies. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "commands.h"
#include "utility.h"

int cd(char *directory) {
    if (directory == NULL) {
        directory = getenv("HOME"); // If no directory is specified, change to the home directory
    }

    if (chdir(directory) != 0) {
        perror("chdir() error");
        return -1;
    }

    // Update the PWD environment variable
    setenv("PWD", directory, 1);

    return 0;
}

int clr() {                                               // Command to clear the Command Line	
    system("clear");
    return 0;
}

int dir(char *args[]) {                                   // Command to list the contents of a directory 
    char cwd[1024], prevwd[1024];
    getcwd(cwd, sizeof(cwd));                             // Gets the current working directory
    getcwd(prevwd, sizeof(cwd));                          // Set previous working directory variable
    if (args[1] != NULL) {                                // if there is a second argument
        int check = chdir(args[1]);                       // Will return -1 if the directory does not exist
        printf("\n");
        if (check > 0) {                                  // if the directory exists
            printf("%s\n", cwd);                          // Print the current working directory
            system("ls -la");                             // Runs the system ls command
            chdir(prevwd);                                // Returns to previous directory
        }
        if (check == -1) {                                // if the directory does not exist
            printf("\nError, no directory named \"%s\" found. Find list of all possible directories above.\n", args[1]);
        }
    } else {                                              // Else just run system ls
        system("ls -la");
    }
    return 0;
}

int list_environment() {
    extern char **environ; // Declare the global variable 'environ'
    char **env = environ;

    while (*env) {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}

void echo(char *message) {
    printf("%s\n", message);
}

int help() {
    system("cat ../manual/readme.md");
    return 0;
}

int pause() {                                       // Command to pause the shell until the user presses enter
    printf("Press Enter to continue...");
    getchar();                                            // Waits for user to press enter
    return 0;
}

int quit() {                                              // Command to exit the shell
    exit(0);
}

void execute(char **args) {
    // Check for input redirection
    char *redirect_input_file = NULL;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            redirect_input_file = args[i + 1];
            args[i] = NULL; // Remove redirection symbol and filename
        }
    }

    // Check for output redirection
    char *redirect_output_file = NULL;
    int output_redirect_index = -1;  // Define new variable to store output redirection index
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            redirect_output_file = args[i + 1];
            args[i] = NULL; // Remove redirection symbol and filename
            output_redirect_index = i;  // Store the index for output redirection
        }
    }

//  https://stackoverflow.com/questions/53256436/difference-of-pid-t-and-int-in-c
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process

        // Handle input redirection
        if (redirect_input_file != NULL) {
            freopen(redirect_input_file, "r", stdin);
        }

        // Handle output redirection
        if (redirect_output_file != NULL) {
            fclose(stdout); // Close current stdout
            if (strcmp(args[output_redirect_index], ">>") == 0) {
                freopen(redirect_output_file, "a", stdout); // Append mode
            } else {
                freopen(redirect_output_file, "w", stdout); // Write mode
            }
        }

    //  https://stackoverflow.com/questions/13494787/calling-ls-with-execve
        if (execvp(args[0], args) == -1) {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
    //  https://linux.die.net/man/2/waitpid
        waitpid(pid, &status, 0);
    }
}