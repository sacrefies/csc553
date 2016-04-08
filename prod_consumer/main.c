// -----------------------------------------------------------------------------
// Filename:    main.cpp
// Revision:    $Id$
// Description: This file includes the implementation of project 5.3:
//              Producer - Consumer Problem
// Created:     04/07/2016 01:20:29 PM
// Compiler:    G++
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/queue.h"
#include "headers/prod_cons.h"
#include "headers/utils.h"


static void print_usage(const char * prog) {
    printf("Usage:\n");
    printf("\t# %s <main_sleep_time> <num_of_producers> <num_of_consumers>\n\n",
           prog);
    printf("Examples:\n");
    printf("\t# %s 10 4 8\n", prog);
    printf("\t# %s 5 3 3\n\n", prog);
}


/**
 * 1. Get command line arguments argv[1],argv[2],argv[3]
 * 2. Initialize buffer
 * 3. Create producer thread(s)
 * 4. Create consumer thread(s)
 * 5. Sleep
 * 6. Exit
 */
int main(const int argc, const char * argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        if (false == is_number(argv[i]) || string_to_int(argv[i]) <= 0) {
            printf("%s - Error: Wrong argument at %d\n", __func__, i);
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int slp = string_to_int(argv[1]),
        prods = string_to_int(argv[2]),
        consms = string_to_int(argv[3]);

    thread_halt = false;

    pthread_t * prod_thrds = (pthread_t *)malloc(sizeof(pthread_t) * prods);
    pthread_t * cons_thrds = (pthread_t *)malloc(sizeof(pthread_t) * consms);

    printf("%s - initializing buffer, size=%d\n", __func__, BUFFER_SIZE);
    queue(BUFFER_SIZE);

    // init semaphores
    if (false == init_sems())
        exit(EXIT_FAILURE);


    for (int i = 0; i < prods + consms; ++i) {
        if (i < prods) {
            int * params = (int *)malloc(sizeof(int));
            params[0] = i;
            printf("%s - creating producer %d\n", __func__, i);
            pthread_create(&prod_thrds[i], NULL, &producer, params);
        } else {
            if (i == prods)
                printf("%s - #############################\n", __func__);
            int * params = (int *)malloc(sizeof(int));
            params[0] = i - prods;
            printf("%s - creating consumer %d\n", __func__, i - prods);
            pthread_create(&cons_thrds[i - prods], NULL, &consumer, params);
        }
    }
    sleep(slp);
    printf("%s - Main thread sleep time has exceeded! Exiting...\n", __func__);
    thread_halt = true;

    // join back the threads, waiting for their quiting
    for (int i = 0; i < prods + consms; ++i) {
        if (i < prods)
            pthread_join(prod_thrds[i], NULL);
        else
            pthread_join(cons_thrds[i - prods], NULL);
    }

    // clean up
    free(prod_thrds);
    free(cons_thrds);
    prod_thrds = NULL;
    cons_thrds = NULL;

    // free semaphores
    if (false == destroy_sems())
        exit(EXIT_FAILURE);

    printf("%s - clear up buffer\n", __func__);
    queue_clear();
    printf("%s - free up buffer space\n", __func__);
    queue_dispose();

    printf("%s - Exiting program\n", __func__);
    pthread_exit(NULL);
    exit(EXIT_SUCCESS);
} /* main */
