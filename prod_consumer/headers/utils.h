// -----------------------------------------------------------------------------
// Filename:    utils.h
// Revision:    $Id$
// Description: This file includes some helper functions
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


/** include guard */
#ifndef UTILS_H
#define UTILS_H

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
char * int_to_string(const int data);


/**
 * Determine whether a string is a number.
 * E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
 *
 * @param str   A string to be examined.
 *
 * @return Returns true if the given string is a legal number;
 *         returns false if otherwise.
 */
bool is_number(const char * str);


/**
 * Convert a string into an integer.
 *
 * @param str A string which represents a number.
 * @return Returns an int
 */
int string_to_int(const char * str);

/**
 * Print the given array to the stdout
 *
 * @param size  The size of the array
 * @param list  The array of type int
 * @param msg   The extra information should be printed.
 */
void print_array(const int size, const int * list, const char * msg);

#endif /** end of include guard */
