// ------------------------------------------------------------------------------------
//       Filename: sum.c
//       Revision: $Id$
//    Description: This file includes a program which uses multithreading to sum up a large
//                 range of numbers.
//        Created: 02/21/2016 01:20:29 PM
//       Compiler: GCC
//         Author: Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng
// ------------------------------------------------------------------------------------

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>


#ifndef PTHREAD_COUNT
#define PTHREAD_COUNT 4
#endif


// A long long value to store the summation.
long long sum;

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

    return (count_digit >= 1);
}


/**
 * Convert a string into a positive int.
 *
 * @param str A string which represents a number.
 * @return Returns an int if conversion is successful.
 */
static int convert(const char str[]) {
    return (int)atof(str);
}


/**
 * Get a slice of a range which is represented by a lower bound and a upper bound.
 *
 * @param slices        How many slices shall be split up?
 * @param slice_index   The index of the desired slice from the number range. Starts with 0.
 * @param lower         The lower bound of a range.
 * @param upper         The upper bound of a range.
 * @param *start        The starting number of the desired slice, one of the return values.
 * @param *end          The ending number of the desired slice, one of the return values.
 */
static void slice(const int slices, const int slice_index, const int lower, const int upper, int *start, int *end) {
    int total_size = abs(upper) - abs(lower) + 1;
    int size_per_piece = total_size / slices;

    if (slice_index >= slices) {
        *end = *start = INT_MIN;
        return;
    }

    *start = lower + slice_index * size_per_piece;
    *end = (slice_index == slices - 1)? upper: *start + size_per_piece - 1;
}


static void *range_sum(void *param) {
    int *p = (int *)param;
    // debug
    printf("start: %d, end: %d\n", p[0], p[1]);
    for (int i = p[0]; i <= p[1]; ++i)
        sum += i;

    pthread_exit(NULL);
}


/**
 * The main entrance of the application.
 */
int main(const int argc, const char *argv[]) {
    char usages[] = "\n[Usages]\n"
                    "$ ./sum <lower_bound> <upper_bound> \nE.g.: ./sum 2 5000\n"
                    "\n[Note]\n"
                    " - If a decimal number is given, say 1.5, the number will be converted to 1.\n";

    // user did not specify a number
    if (argc < 3) {
        printf(usages);
        return 0;
    }

    char *lower = strdup(argv[1]),
         *upper = strdup(argv[2]);

    if (!isnumber(lower)) {
        printf("Illegal input: %s\nPlease specify a lower bound.\n", lower);
        printf(usages);
        return -1;
    }
    if (!isnumber(upper)) {
        printf("Illegal input: %s\nPlease specify an upper bound.\n", upper);
        printf(usages);
        return -1;
    }

    int upper_bound = convert(upper),
        lower_bound = convert(lower);

    if (upper_bound <= lower_bound) {
        printf("Illegal input: %s %s\nPlease specify a valid range.\n", lower, upper);
        printf(usages);
        return -1;
    }
    // debug
    printf("user input: lower=%d upper=%d\n", lower_bound, upper_bound);
    // prime searching begins
    int rc;
    int params[2];
    // create the thread addresses on heap.
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * PTHREAD_COUNT);

    for (long tid = 0; tid < PTHREAD_COUNT; ++tid) {
        slice(PTHREAD_COUNT, (int)tid, lower_bound, upper_bound, &params[0], &params[1]);
        if (params[0] == INT_MIN && params[1] == INT_MIN) {
            printf("Error: Failed to slice the range. for thread %ld\n", tid);
            continue;
        }
        rc = pthread_create(&threads[tid], NULL, &range_sum, (void *)params);
        pthread_join(threads[tid], NULL);
        if (rc != 0)
            printf("Error: Failed to create or run thread %ld, return code: %d", tid, rc);
    }

    printf("The summary of [%d..%d]: %lld\n", lower_bound, upper_bound, sum);

    pthread_exit(NULL);
    return 0;
}
