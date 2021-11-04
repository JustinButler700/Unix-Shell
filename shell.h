/*=========================================================
//HW#: Assignment 2
//Your name: David Farjon & Justin Butler
// Date : October 2021
//Complier:  GCC
//File type: Header
//===========================================================*/
#ifndef SHELL_H
#define SHELL_H

#define TRUE 1
#define ERROR -1
#define SEPARATOR 32 // SPACE CHAR in Decimal

//Create a new shell structure
typedef struct shell_s
{
    char *str;
    char *last;
    char **tab;
} shell_t;

#endif /* SHELL_H */