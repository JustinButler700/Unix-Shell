This is a C shell terminal program. Pipelining, History, and IO redirection are all included features we've implemented. 
Made by Justin Butler & David Farjon

How to Compile and Run the Program:

To compile the program, use the command: make

To run the program, use the command: ./shell

Useful commands:

Run last executed: !!
Io Redirection right: >
example: ls > test
Io Redirection left: <
example: sort -nr < test


files:

    • Shell.h: A header file that holds the shell structure & defines separator char and others

    • main.c: A source file that creates a shell program

    • Makefile: This allows us to compile the above files using the make instruction to produce an executable file “shell”, also creates executables (.o) for all of the source files


Features Implemented (by class):
    • Main (shell)
        ◦ Putstr
            ▪ prints a string to the console.
        ◦ Tablen
            ▪ returns the length of a array of character arrays.
        ◦ freetab
            ▪ frees the memory at the current address (array of char arrays)
        ◦ set_shell
            ▪ set passed shell to null
        ◦ reset_shell
            ▪ resets the shell
        ◦ malloc_tab
            ▪ Allocations ram for all elements in the list of string + 1 for \0
        ◦ split
            ▪ split the string into a list of strings based on seperations (" ")
        ◦ exec
            ▪ Basic execution call of a shell command based on input string
        ◦ remove_tab
            ▪ if there is a tab on the shell, we replace it with a blank spot since it is the preferred separator.
        ◦ delete_space
            ▪ Removes excess space from string.
        ◦ check_sign
            ▪ check for a '<' or '>' or '|' for rediretion/piping in the char array.
            ▪ Returns an id representing '>', '<', or '|' being 1, 2, or 3.
        ◦ exec_pipe_step
            ▪ Execute the left side of the command line
        ◦ exec_pipe
            ▪ Split the strings based off separator & executed left & right side with piping.
        ◦ exec_right_arrow
            ▪ Implement right I/O redirection
        ◦ exec_left_arrow
            ▪ Implement left I/O redirection.
        ◦ command_center
            ▪ check if we call history & checks for semicolons to split the string at.
        ◦ Shell
            ▪ Constructs the shell prompt and runs the main loop of the program to get/check the command line.
    • Shell.h
        ◦ Creates the shell struct
Design and Implementation choices:
	The way we chose to write our shell is like a factory, we are iteratively processing the passed in string from the shell to each function. Each function determines if the answer meets the necessary criteria to be passed to its next function. Like a conveyer belt in order, each process is linked to its passed function call. We implemented piping as an additional feature and multiple commands in the same line. (;) 


Lessons Learned / re-Learned
	We tried using a split string function with the strtok_r but we had leaks and it was not properly optimized. So, we created our own splitter method where it only calls one standard library function (malloc). This function call is more focused for our efficacy. Lastly, we were having trouble for a while because the second argument of execvp was wrong. We passed it the wrong variable because all variables in the function had similar naming conventions it was initially overlooked. (cmda, cmdb, cmd). Luckily, after asking the professor about this he was able to find this small mistake. This issue was very trivial, but in the future our naming conventions may be something to give more thought to.
	
References:
	 
	We used a C manual to properly implement the necessary library calls for our shell.