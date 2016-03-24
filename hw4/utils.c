// -----------------------------------------------------------------------------
// Filename:    utils.c
// Revision:    $Id$
// Description: This file includes some helper functions
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/utils.h"


/**
 * Determine whether a string is a number.
 * E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
 *
 * @param str A string to be examined.
 * @return Returns true if the given string is a legal number;
 * returns false if otherwise.
 */
bool isNumber(const char * str) {
    if (str == NULL)
        return false;

    int size = strlen(str);
    if (size == 0)
        return false;

    int count_dot = 0, count_sign = 0, count_digit = 0;
    for (int i = 0; i < size; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            if (str[i] != '+' && str[i] != '.' && str[i] != '-')
                return false;
            // only allow + or - existing as a sign symbol
            if (str[i] == '+' || str[i] == '-') {
                if (count_sign > 0)
                    return false;
                count_sign++;
            }
            // only allow one dot
            if (str[i] == '.') {
                if (count_dot > 0)
                    return false;
                count_dot++;
            }
        } else
            count_digit++;
    }

    return (count_digit > 0);
} /* isNumber */


/**
 * Convert a string into an integer.
 *
 * @param str A string which represents a number.
 * @return Returns an int
 */
int toInt(const char * str) {
    return (int)atof(str);
}


/**
 * Print the given array to the stdout
 *
 * @param size  The size of the array
 * @param list  The array of type int
 * @param msg   The extra information should be printed.
 */
void printArray(const int size, const int * list, const char * msg) {
    printf("[");
    if (list != NULL) {
        for (int i = 0; i < size; ++i) {
            printf("%d", list[i]);
            if (i < size - 1)
                printf(",");
        }
    }
    printf("]\n");
}
