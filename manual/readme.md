Name
    MyShell

Description
    MyShell is a simple command-line shell. 
    It offers basic functionality similar to UNIX shells, such as executing commands 
    and managing the environment.

Commands
    - cd: Change the current working directory. Usage: `cd [directory]`. 
            If no argument given will change to
            Home Directory
    - clr: Clear the screen.
    - dir: List the contents of a directory. 
    Usage: `dir [directory]`.
            If no argument given will list
            contents of current directory
    - environ: Display all environment variables.
    - echo: Display a message. Usage: `echo [message`.
    - help: Display the shell manual.
    - pause: Pause shell execution until the Enter
            key is pressed.
    - quit: Exit the shell.

Environment Concept
    MyShell updates environment variables such as SHELL and PWD to reflect the current shell 
    environment and working directory, respectively.

Batchfile
    MyShell supports batch file execution. Use the command `./myshell batchfile` to execute 
    commands from a file.

I/O Redirection
    MyShell supports basic I/O redirection, allowing redirection of stdin and/or stdout. However, 
    due to implementation limitations, commands and
    arguments passed will only work inside the shell
    enviornment.

    I/O redirection in MyShell allows you to control the flow of input and output streams between commands and files. You can redirect standard input (stdin) and standard output (stdout) using the following symbols:

    <: Redirects stdin from a file.
    >: Redirects stdout to a file.
    >>: Redirects stdout to a file 
    (appends if the file exists).

Process Execution
    MyShell allows both foreground and background command execution. Background commands can be 
    executed by appending `&` to the command.

References
    - G. Healy, "Lab04/05: Building a Shell". Available at: https://loop.dcu.ie/mod/book/view.php?id=2322719&chapterid=492179 [Acessed: 15-Mar-2024]
    - W. Joy, "An introduction to the C shell". Available at: https://docs-archive.freebsd.org/44doc/usd/04.csh/paper.html [Accessed: 15-Mar-2024]
    -"Fork() in C". Available at: https://www.geeksforgeeks.org/fork-system-call/ [Accessed: 16-Mar-2024]
    - A. Silberschatz, P. B. Galvin & G. Gagne, "Operating systems Concepts" [2018]
    - "IBM Documentation - Processes". Available at: https://www.ibm.com/docs/en/aix/7.1?topic=processes- [Accessed: 18-Mar-2024].


/* 
Name: Adam McKeown
Student Number: 22462626
I, Adam McKeown, acknowledge all of DCU's Academic Integrity Policies. 
*/