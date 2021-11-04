/*=========================================================
//HW#: Assignment 2
//Your name: David Farjon & Justin Butler
// Date : October 2021
//Complier:  GCC
//File type: client program
//===========================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

//PURPOSE: prints a string to the console
//PARAMETER: str - string to be printed
void putstr(char *str)
{
    for (int i = 0; str[i] != 0; i++) // writes char by char
        write(1, &str[i], 1);         // printf bufferizes & can have problems, so to prevent display issues we only use write.
}

//PURPOSE: returns the length of a array of character arrays
//PARAMETER: tab, a table of our current strings
//Output: i, the length of the table
int tablen(char **tab)
{
    int i = 0;

    if (tab == NULL)
        return (0);
    while (tab[i] != NULL)
        i++;
    return (i);
}
//Purpose: frees the memory at the current address (array of char arrays)
//Parameter: an address (array of char arrays)
void freetab(char **tab)
{
    if (tab == NULL)
        return;
    for (int i = 0; i != tablen(tab); i++)
        free(tab[i]);
    free(tab);
}
//PURPOSE: set passed shell to null
//PARAMETER: sh current shell
void set_shell(shell_t *sh)
{
    sh->str = NULL;
    sh->last = NULL;
    sh->tab = NULL;
}

//PURPOSE: resets the shell
//PARAMETER: sh, the current shell structure which is being reset
void reset_shell(shell_t *sh)
{
    if (sh->tab != NULL)
        freetab(sh->tab); //free the memory
    sh->tab = NULL;
}

//Purpose: Allocations ram for all elements in the list of string + 1 for \0
//Output: returns a table/array with size pre-allocated
char **malloc_tab(char *str, char separator, int *size)
{
    char **tab;

    for (int i = 0; str[i] != 0;)
    {
        while (str[i] == separator)
            i++;
        while (str[i] != separator && str[i] != 0)
            i++;
        if ((str[i] == 0 && str[i - 1] != separator) ||
            str[i] != 0)
            (*size)++;
    }
    tab = malloc(sizeof(char *) * ((*size) + 1)); //allocation of memory
    tab[(*size)] = NULL;
    for (int i = 0; i != *size; i++)
        tab[i] = malloc(sizeof(char) * ((strlen(str) / (*size)) + 1)); //allocation of memory +1 (for null)
    return (tab);
}
//Purpose: split the string into a list of strings based on seperations (" ")
//Output: an array of char arrays filled with elements of str
char **split(char *str, char separator)
{
    int size = 0;
    char **tab = malloc_tab(str, separator, &size); // allocates for size
    int compt = 0;
    int bloc = 0;
    int check = 0;
    int i = 0;
    //fill the table
    while (bloc != size) // goes from 0 -> size
    {
        while (str[i] == separator) // skip over separator chars
            i++;
        while (str[i] != separator && str[i] != 0) //filling the array
        {
            tab[bloc][compt] = str[i];
            compt++;
            i++;
            check++;
        }
        if (check > 0) // if we contain an element, add a reset \0 to the end index & reset variables
        {
            tab[bloc][compt] = 0;
            compt = 0; // all strings start at [0] so we reset
            bloc++;    // set to go to next table
            check = 0;
        }
    }
    return (tab);
}
//This split uses a string of seperator
// char **split(char *str)
// {
//     char **tab;
//     char *copy = NULL, *line = NULL, *brk = NULL;
//     int size = 1;
//     int i = 0;
//     copy = strdup(str);
//     for (line = strtok_r(copy, SEPARATOR, &brk);
//         line;
//         line = strtok_r(NULL, SEPARATOR, &brk))
//         size++;
//     tab = malloc(sizeof(char *) * size);
//     tab[size] = NULL;
//     free(copy);
//     copy = strdup(str);
//     for (line = strtok_r(copy, SEPARATOR, &brk);
//         line;
//         line = strtok_r(NULL, SEPARATOR, &brk)) {
//         tab[i] = strdup(line);
//         i++;
//     }
//     for (int i = 0; tab[i] != NULL; i++)
//         printf("line %d: [%s]\n", i, tab[i]);
//     return (tab);
// }

//Purpose: Basic execution call of a shell command based on input string
//Parameter: takes a char array to execute
static int exec(char *str)
{
    int wstatus = 0;
    pid_t pid = 0;
    char **cmd = split(str, SEPARATOR); // split the string in array of char array by " "

    pid = fork();
    wait(&wstatus);
    if (pid == -1)
        perror("Fork failed"); // write string in the perror & will automatically follow with the log of the error
    if (pid == 0)
    {
        if (execvp(cmd[0], cmd) == -1) //executes the command and sees if the command is found
        {
            putstr(cmd[0]);
            putstr(": Command not found.\n");
            exit(-1); // exits the child fork
        }
        freetab(cmd); // useless memory free
    }
    return (0); // if fork is successful, return.
}
//Purpose: if there is a tab on the shell, we replace it with a blank spot since it is the preferred seperator
//Parameter: the current pointer to the shell
void remove_tab(shell_t *sh)
{
    if (sh->str == NULL)
        return;
    for (int i = 0; sh->str[i] != 0; i++)
        if (sh->str[i] == '\t')
            sh->str[i] = ' ';
}
//Purpose: Removes excess space
//Parameter: current string to remove space from
char *delete_space(char *str)
{
    char *new = NULL;
    int space = 0;
    int max_char = strlen(str);
    int i = 0;

    while (str[space] == ' ')
        space++;
    if (space == 0)
        return (str);
    max_char -= space;
    new = malloc(sizeof(char) * (max_char + 1));
    new[max_char] = '\0';
    while (i != max_char)
    {
        new[i] = str[space];
        i++;
        space++;
    }
    free(str);
    return (new);
}
//Purpose: check for a '<' or '>' or '|' for rediretion/piping in the char array
//Parameter: string
//Output: an int id representing '>', '<', or '|' 1, 2, or 3.
int check_sign(char *str)
{
    if (strlen(str) < 5)
        return (0);
    for (int i = 1; str[i + 1] != 0; i++)
    {
        if (str[i - 1] == ' ' && str[i + 1] == ' ')
        {
            if (str[i] == '>')
                return (1);
            if (str[i] == '<')
                return (2);
            if (str[i] == '|')
                return (3);
        }
    }
    return (0);
}
//Purpose: Execute the left side of the command line
//Parameter: take a 2 file descriptors, the left side of the passed command
void exec_pipe_step(int fda, int fdb, char **cmdz)
{
    close(STDOUT_FILENO);
    dup(fdb);
    close(fda);
    close(fdb);
    execvp(cmdz[0], cmdz);
    perror("execvp failed");
    exit(1);
}
//Purpose: Splits the commands based on the seperator '|' and executres the left & right side
//Parameter: the current string
void exec_pipe(char *str)
{
    int ws[2];
    char **cmd = split(str, '|');
    char **cmda = split(cmd[0], SEPARATOR);
    char **cmdb = split(cmd[1], SEPARATOR);

    if (pipe(ws) == -1)
    {
        perror("Pipe failed");
        exit(1);
    }
    if (fork() == 0)
        exec_pipe_step(ws[0], ws[1], cmda);
    if (fork() == 0)
    {
        //exec_pipe_step(ws[1], ws[0], cmdb); // Don't work
        close(STDIN_FILENO);
        dup(ws[0]);
        close(ws[1]);
        close(ws[0]);
        execvp(cmdb[0], cmdb);
        perror("execvp failed");
        exit(1);
    }
    close(ws[0]);
    close(ws[1]);
    wait(0);
    wait(0);
    freetab(cmd);
    freetab(cmda);
    freetab(cmdb);
}
//PURPOSE: Implements right redirection on a parsed string input
//PARAMETER: str. The string input from terminal which is to be parsed.
/*
Redirection allows commands' file handles to be duplicated, opened, closed,
made to refer to different files, and can change the files the command reads 
from and writes to.
*/
void exec_right_arrow(char *str)
{
    char **cmd = split(str, '>');
    char **cmda = split(cmd[0], SEPARATOR);
    char **cmdb = split(cmd[1], SEPARATOR);
    int fd = 0; // file descriptor
    int wstatus = 0;
    pid_t pid = 0;

    pid = fork();
    wait(&wstatus);
    if (pid == -1)
        perror("Fork failed");
    if (pid == 0)
    {
        fd = open(cmdb[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execvp(cmda[0], cmda) == -1)
        {
            putstr(cmda[0]);
            putstr(": Command not found.\n");
            exit(-1);
        }
        freetab(cmda);
        freetab(cmdb);
        freetab(cmd);
    }
}

//PURPOSE: Implements left redirection on a parsed string input
//PARAMETER: str. The string input from terminal which is to be parsed.
/*
Redirection allows commands' file handles to be duplicated, opened, closed,
made to refer to different files, and can change the files the command reads 
from and writes to.
*/
void exec_left_arrow(char *str)
{
    char **cmd = split(str, '<'); // splits string between command name & file name
    char **cmda = split(cmd[0], SEPARATOR);
    char **cmdb = split(cmd[1], SEPARATOR);
    int fd = 0; // file descriptor
    int wstatus = 0;
    pid_t pid = 0;

    pid = fork();
    wait(&wstatus);
    if (pid == -1)
        perror("Fork failed");
    if (pid == 0)
    {
        fd = open(cmdb[0], O_RDONLY, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        if (fd == ERROR)
        {
            perror(cmdb[0]);
            exit(-1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
        if (execvp(cmda[0], cmda) == -1)
        {
            putstr(cmda[0]);
            putstr(": Command not found.\n");
            exit(-1);
        }
        freetab(cmda);
        freetab(cmdb);
        freetab(cmd);
    }
}
// Purpose: Check if we call history, and if not, we split the string by semicolons until the last command.
//          for each semicolon, we call their specific execution functions.
// Parameter: The current shell structure
void command_center(shell_t *sh)
{
    if (sh->last != NULL && !strcmp(sh->str, "!!"))
        sh->tab = split(sh->last, ';');
    else
        sh->tab = split(sh->str, ';');
    for (int i = 0; i != tablen(sh->tab); i++)
    {
        if (check_sign(sh->tab[i]) == 0)
            exec(sh->tab[i]);
        if (check_sign(sh->tab[i]) == 1)
            exec_right_arrow(sh->tab[i]);
        if (check_sign(sh->tab[i]) == 2)
            exec_left_arrow(sh->tab[i]);
        if (check_sign(sh->tab[i]) == 3)
            exec_pipe(sh->tab[i]);
    }
}
//Purpose: the main loop of the program with the prompt & a function to get/check the current command line
//Output: return statement
static int shell(shell_t *sh)
{
    int check = 0;
    size_t size = 0;

    while (TRUE)
    {
        reset_shell(sh);
        write(1, "\033[36;1;4;5;7m\\o/\033[0m \033[31;1m❯\033[0m", 33);
        write(1, "\033[33;1m❯\033[0m\033[34;1m❯\033[0m ", 29);
        check = getline(&sh->str, &size, stdin);
        if (check == -1)
            break;
        sh->str[check - 1] = 0;
        remove_tab(sh);
        sh->str = delete_space(sh->str);
        check = 0;
        if (!strcmp(sh->str, "exit"))
            break;
        command_center(sh);
        if (sh->last != NULL)
            free(sh->last);
        sh->last = strdup(sh->str);
    }
    free(sh->last);
    reset_shell(sh);
    return (0); // ends main
}
// Start the loop of the program
int main(void)
{
    shell_t sh;
    set_shell(&sh); // Initiate shell structure
    return (shell(&sh));
}
