#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h> // Include for signal handling
#include "commands.h"
#include "utility.h"

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

void setup_shell_environment();

void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char **argv, char *envp[]) {
    char input_buffer[MAX_BUFFER];
    char *args[MAX_ARGS];

    setup_shell_environment();  // Set up the shell environment

    // Register SIGCHLD signal handler
    signal(SIGCHLD, sigchld_handler);

    // Check if the shell is running interactively
    int interactive = isatty(STDIN_FILENO);

    if (interactive) {
        welcome();
    }

    if (argc == 2) {
        // Execute batch file if provided as argument
        FILE *file = fopen(argv[1], "r");
        if (file != NULL) {
            if (interactive) {
                prompt();  // Print prompt only if running interactively
            }
            while (fgets(input_buffer, MAX_BUFFER, file) != NULL) {
                // Remove the newline character from input_buffer if it exists
                input_buffer[strcspn(input_buffer, "\n")] = '\0';

                char **command = splitline(input_buffer);
                if (command == NULL) {
                    fprintf(stderr, "Error: Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                printf("Executing command: %s\n", *command);

                // Check for redirection symbols
                char *redirect_input_file = NULL;
                char *redirect_output_file = NULL;
                int append_mode = 0;  // 1 if >> is used for appending

                for (int i = 0; command[i] != NULL; i++) {
                    if (strcmp(command[i], "<") == 0) {
                        redirect_input_file = command[i + 1];
                        command[i] = NULL;  // Remove redirection symbol and filename from command
                    } else if (strcmp(command[i], ">") == 0) {
                        redirect_output_file = command[i + 1];
                        append_mode = 0;  // Set to write mode
                        command[i] = NULL;
                    } else if (strcmp(command[i], ">>") == 0) {
                        redirect_output_file = command[i + 1];
                        append_mode = 1;  // Set to append mode
                        command[i] = NULL;
                    }
                }

                // Check if command is internal command
                if (strcmp(command[0], "cd") == 0) {
                    // Handle "cd" command separately
                    if (cd(command[1]) != 0) {
                        perror("cd command failed");
                    }
                } else if (strcmp(command[0], "environ") == 0) {
                    // Handle "environ" command
                    list_environment();
                } else if (strcmp(command[0], "pause") == 0) {
                    // Handle "pause" command
                    pause();
                } else if (strcmp(command[0], "quit") == 0) {
                    // If the command is "quit", exit the shell
                    exit(EXIT_SUCCESS);
                } else {
                    // Execute other commands
                    if (redirect_output_file != NULL) {
                        // Redirect stdout to output file
                        FILE *output_stream = freopen(redirect_output_file, append_mode ? "a" : "w", stdout);
                        if (output_stream == NULL) {
                            perror("Failed to redirect output");
                            exit(EXIT_FAILURE);
                        }
                    }
                    execute(command); // Execute command
                    if (redirect_output_file != NULL) {
                        fclose(stdout); // Close redirected stdout
                        stdout = fdopen(1, "w"); // Restore stdout
                    }
                }
                free(command); // Free memory allocated for command
            }
            fclose(file);
            exit(EXIT_SUCCESS);  // Exit after batch mode execution
        } else {
            perror("Error opening batch file");
            exit(EXIT_FAILURE);
        }
    }

    while (1) {
        // Main shell loop
        if (interactive) {
            prompt();
        }
        
        if (fgets(input_buffer, MAX_BUFFER, stdin)) {
            char **arg = args;
            *arg++ = strtok(input_buffer, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)));

            if (args[0] != NULL) {
                // Check for background execution
                if (hasampersand(args)) {
                    // Run the command in the background
                    backgroundexecute(args);
                } else {
                    // Run the command in the foreground
                    // Check for redirection symbols
                    char *redirect_input_file = NULL;
                    char *redirect_output_file = NULL;
                    int append_mode = 0;  // 1 if >> is used for appending

                    for (int i = 0; args[i] != NULL; i++) {
                        if (strcmp(args[i], "<") == 0) {
                            redirect_input_file = args[i + 1];
                            args[i] = NULL;  // Remove redirection symbol and filename from args
                        } else if (strcmp(args[i], ">") == 0) {
                            redirect_output_file = args[i + 1];
                            append_mode = 0;  // Set to write mode
                            args[i] = NULL;
                        } else if (strcmp(args[i], ">>") == 0) {
                            redirect_output_file = args[i + 1];
                            append_mode = 1;  // Set to append mode
                            args[i] = NULL;
                        }
                    }

                    if (strcmp(args[0], "quit") == 0) {
                        // If the command is "quit", simply exit the shell
                        exit(EXIT_SUCCESS);
                    } else if (strcmp(args[0], "cd") == 0) {
                        // Handle "cd" command separately
                        if (cd(args[1]) != 0) {
                            perror("cd command failed");
                        }
                    } else if (strcmp(args[0], "environ") == 0) {
                        // Handle "environ" command
                        list_environment();
                    } else if (strcmp(args[0], "help") == 0) {
                        // Handle "help" command
                        help();
                    } else if (strcmp(args[0], "pause") == 0) {
                        // Handle "pause" command
                        pause();
                    } else {
                        // Execute other commands
                        if (redirect_input_file != NULL) {
                            // Redirect stdin from input file
                            FILE *input_stream = freopen(redirect_input_file, "r", stdin);
                            if (input_stream == NULL) {
                                perror("Failed to redirect input");
                                exit(EXIT_FAILURE);
                            }
                        }

                        if (redirect_output_file != NULL) {
                            // Redirect stdout to output file
                            FILE *output_stream = freopen(redirect_output_file, append_mode ? "a" : "w", stdout);
                            if (output_stream == NULL) {
                                perror("Failed to redirect output");
                                exit(EXIT_FAILURE);
                            }
                        }

                        // Execute the command
                        execute(args);

                        // Reset stdin and stdout to terminal
                        freopen("/dev/tty", "r", stdin);
                        freopen("/dev/tty", "w", stdout);
                    }
                }
            }
        }
    }

    return 0;
}

void setup_shell_environment() {
    // Set up the shell environment
    char shell_path[MAX_BUFFER];
    strcpy(shell_path, getenv("PWD"));
    strcat(shell_path, "/theshell");
    setenv("SHELL", shell_path, 1);
}
