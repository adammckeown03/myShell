/* 
Name: Adam McKeown
Student Number: 22462626
I, Adam McKeown, acknowledge all of DCU's Academic Integrity Policies. 
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>

/* Function prototypes */
time_t getTime();
void welcome();
void prompt();
void execute(char **args);
char **splitline(char *input);
int hasampersand(char *args[]);
int backgroundexecute(char **args);

#endif /* UTILITIES_H */