/* 
Name: Adam McKeown
Student Number: 22462626
I, Adam McKeown, acknowledge all of DCU's Academic Integrity Policies. 
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>
#include <unistd.h>

#define MAX_COMMAND_SIZE 1024

int cd(char *directory);
int clr();
int dir(char *args[]);
int list_environment();
void echo(char *message);
int help();
int pause();
int quit();
void execute(char **args);

#endif /* COMMANDS_H */