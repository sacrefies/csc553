// ------------------------------------------------------------------------------------
//       Filename: primes.c
//       Revision: $Id$
//    Description: This file includes implementation of problem 4.24 from the textbook.
//        Created: 02/21/2016 01:20:29 PM
//       Compiler: GCC
//         Author: Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng
// ------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef PTHREAD_COUNT
#define PTHREAD_COUNT 1
#endif

/**
 * Determine whether a string is a number.
 * E.g.: "5", "+5", "-5", "3.5", ".5", "5." are legal number forms.
 *
 * @param str A string to be examined.
 * @return Returns true if the given string is a legal number;
 * returns false if otherwise.
 */
static bool isnumber(const char str[]) {
    if (str == NULL)
        return false;

    int size = strlen(str);
    int count_dot = 0, count_sign = 0, count_digit = 0;
    if (size == 0)
        return false;

    for (int i = 0; i < size; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            if (str[i] != '+' && str[i] != '.' && str[i] != '-')
                return false;
            if (str[i] == '+' || str[i] == '-') {
                if ((i > 0 && i < size - 1) || count_sign > 0) return false;
                count_sign ++;
            }
            if (str[i] == '.') {
                if (count_dot > 0) return false;
                count_dot ++;
            }
        } else
            count_digit ++;
    }

    return (count_digit < 1)? false: true;
}


/**
 * Convert a string into a positive int. Returns -1 if the number
 * represented by the specified string is less than 2 or
 * greater than 100000.
 *
 * @param str A string which represents a number.
 * @return Returns an int if conversion is successful; returns -1
 * if the number represented by the specified string is less than
 * 2 or greater than 100000.
 */
static int convert(const char str[]) {
    int rs = (int)atof(str);
    return (rs < 2 || rs > 100000)? -1: rs;
}


/**
 * The routine for a thread.
 * This method searches for all possible prime numbers
 * which are less than or equal to the specified number.
 *
 * @param params Only the first param is taken by the method
 *               which should be a number.
 */
static void *search_primes(void *params) {
    int upper_bound = ((int *)params)[0];
    // debug
    printf("param[0]: %d\n", upper_bound);

    // bool markers[] = (bool)malloc(sizeof(bool) * upper_bound)
    bool is_prime;
    for (int i = 2; i <= upper_bound; ++i) {
        is_prime = true;
        // for (int j = 2; j < i; ++j) {
        for (int j = 2; j < i / 2; ++j) {
            // if (i % j == 0 && j != i) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            printf("%d,", i);
    }
    printf("\n");
    pthread_exit(NULL);
}


/**
 * The main entrance of the application.
 */
int main(const int argc, const char *argv[]) {
    char usages[] = "\n[Usages]\n"
                    "$ ./primes <A positive integer>\nE.g.: ./primes 21\n"
                    "\n[Note]\n"
                    " - If a decimal number is given, say 1.5, the number will be converted to 1;\n"
                    " - Any number whose converted result is less than 2 is not accepted;\n"
                    " - Any number whose converted result is greater than 100,000 is not accepted.\n";

    // user did not specify a number
    if (argc < 2) {
        printf(usages);
        return 0;
    }

    char *number = strdup(argv[1]);
    // debug
    printf("raw input: %s\n", number);
    if (!isnumber(number)) {
        printf("Illegal input: %s\nPlease specify a number.\n", number);
        printf(usages);
        return -1;
    }

    int prime_upper_bound = convert(number);
    if (prime_upper_bound < 0) {
        printf("The spcified number is either less than 2 or greater than 100000:"
            " %s\nPlease specify a number.\n", number);
        printf(usages);
        return -1;
    }
    // debug
    printf("user input: %d\n", prime_upper_bound);
    // prime searching begins
    int rc;
    pthread_t threads[PTHREAD_COUNT];
    for (long tid = 0; tid < PTHREAD_COUNT; ++tid) {
        rc = pthread_create(&threads[tid], NULL, &search_primes, &prime_upper_bound);
        if (rc != 0)
            printf("Error: failed to create or run thread %ld, return code: %d", tid, rc);
    }

    pthread_exit(NULL);
    return 0;
}
