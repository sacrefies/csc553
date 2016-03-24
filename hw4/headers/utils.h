// -----------------------------------------------------------------------------
// Filename:    utils.h
// Revision:    $Id$
// Description: This header file defines some helper functions
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


/** include guard:  */
#ifndef UTILS_H
#define UTILS_H

/**
 * Determine whether a string is a number.
 * E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
 *
 * @param str A string to be examined.
 * @return Returns true if the given string is a legal number;
 * returns false if otherwise.
 */
bool isNumber(const char * str);


/**
 * Convert a string into an integer.
 *
 * @param str A string which represents a number.
 * @return Returns an int
 */
int toInt(const char * str);


/**
 * Print the given array to the stdout
 *
 * @param size  The size of the array
 * @param list  The array of type int
 * @param msg   The extra information should be printed.
 */
void printArray(const int size, const int * list, const char * msg);

#endif /** end of include guard:  */
