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

// for testing
#include <time.h>


/**
 * Convert an integer, base 10, to its C string representation with its exact
 * length.
 *
 * Note: the output C string is created on heap, so caller doesn't have to
 *       maintain a string buffer.
 *       The caller must manually destroy the C string created by this function.
 * Usage:
 *     // rep == '32082\0'
 *     char * rep = int_to_string(32082);
 *     // rep == '-39\0'
 *     char * rep = int_to_string(-39);
 */
char * int_to_string(const int data) {
    int internal, count = 0;
    char * out = NULL;
    bool neg = false;

    if (data == 0) {
        out = (char *)malloc(sizeof(char) * 2);
        out[0] = '0';
        out[1] = '\0';
        return out;
    }

    internal = data;
    if (data < 0) {
        count++;
        neg = true;
        internal = abs(data);
        //printf("%s - abs(internal)=%d\n", __func__, internal);
    }

    while (internal > 0) {
        // get remaining digits
        internal /= 10;
        count++;
        //printf("%s - inter=%d, count=%d\n", __func__, internal, count);
    }

    // digits = (int *)malloc(sizeof(int) * count);
    out = (char *)malloc(sizeof(char) * (count + 1));
    // set the ending
    out[count] = '\0';
    internal = abs(data);
    //printf("%s - before conversion, inter=%d, count=%d\n", __func__, internal, count);
    for (int i = count - 1; i >= 0; --i ) {
        if (i == 0 && true == neg) {
            out[i] = '-';
        } else {
            out[i] = internal % 10 + '0';
            // get remaining digits
            internal /= 10;
        }
        //printf("%s - out[%d]=%c\n", __func__, i, out[i]);
    }
    //printf("%s - output string len=%d\n", __func__, (int)strlen(out));
    return out;

} /* int_to_string */


/**
 * Determine whether a string is a number.
 * E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
 *
 * @param str   A string to be examined.
 *
 * @return Returns true if the given string is a legal number;
 *         returns false if otherwise.
 */
bool is_number(const char * str) {
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
} /* is_number */


/**
 * Convert a string into an integer.
 *
 * @param str A string which represents a number.
 * @return Returns an int
 */
int string_to_int(const char * str) {
    return (int)atof(str);
}


/**
 * Print the given array to the stdout
 *
 * @param size  The size of the array
 * @param list  The array of type int
 * @param msg   The extra information should be printed.
 */
void print_array(const int size, const int * list, const char * msg) {
    printf("%s [", msg);
    if (list != NULL) {
        for (int i = 0; i < size; ++i) {
            printf("%d", list[i]);
            if (i < size - 1)
                printf(",");
        }
    }
    printf("]\n");
}
