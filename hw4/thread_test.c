// -----------------------------------------------------------------------------
// Filename:    thread_test.c
// Revision:    $Id$
// Description: This file including functions to test POSIX multithreading
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef PTHREAD_COUNT
#define PTHREAD_COUNT      5
#define THREAD_SLEEP_COUNT 5
#define THREAD_SLEEP_SEC   1
#endif

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


static void print_array(const int * array, const int size, const char * tag) {
    printf("%s - %s [", tag, __func__);

    for (unsigned int i = 0; i < size; ++i) {
        printf("%d", array[i]);
        if (i < size - 1)
            printf(",");
    }

    printf("]\n");
}


static void * runner(void * params) {
    int * para_list = (int *)params;

    // print_array(para_list, 2, __func__);

    int tid = para_list[0];
    int sec = (para_list[1] > 0) ? para_list[1] : THREAD_SLEEP_SEC;

    printf("%s - Params received: tid=%d, sec=%d\n", __func__, tid, sec);
    for (int i = 0; i < THREAD_SLEEP_COUNT; ++i) {
        printf("%s - Thread %d sleep for %d sec\n", __func__, tid, sec);
        sleep(sec);
        if (para_list[1] <= 0)
            break;
    }
    pthread_exit(NULL);
}


static void * runner_with_lock(void * params) {
    int * para_list = (int *)params;

    // print_array(para_list, 2, __func__);

    int tid = para_list[0];
    int sec = (para_list[1] > 0) ? para_list[1] : THREAD_SLEEP_SEC;

    pthread_mutex_lock(&mutex);
    printf("%s - Params received: tid=%d, sec=%d\n", __func__, tid, sec);
    for (int i = 0; i < THREAD_SLEEP_COUNT; ++i) {
        printf("%s - Thread %d sleep for %d sec\n", __func__, tid, sec);
        sleep(sec);
        if (para_list[1] <= 0)
            break;
    }
    pthread_mutex_lock(&mutex);
    pthread_exit(NULL);
}


static void run_threads_join_later() {
    // define threads
    printf("%s - Define threads: runner() * %d\n", __func__, PTHREAD_COUNT);
    pthread_t * threads = (pthread_t *)malloc(sizeof(pthread_t) * PTHREAD_COUNT);

    // create threads
    int rc;
    for (int i = 0; i < PTHREAD_COUNT; ++i) {
        int params[2] = { i, 3 };
        print_array(params, 2, __func__);
        printf("%s - Start thread %d\n", __func__, params[0]);
        rc = pthread_create(&threads[i], NULL, &runner, params);
        if (rc != 0) {
            printf("%s - Failed to start thread %d, RC = %d\n",
                   __func__, params[0], rc);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < PTHREAD_COUNT; ++i) {
        printf("%s - Join thread %d\n", __func__, i);
        rc = pthread_join(threads[i], NULL);
        if (rc != 0)
            printf("%s - Failed to join thread %d, RC=%d\n", __func__, i, rc);
    }
    // clean up
    free(threads);
    threads = NULL;

} /* run_threads_join_later */


static void run_threads_join_later_lock() {
    // define threads
    printf("%s - Define threads: runner_with_lock() * %d\n", __func__, PTHREAD_COUNT);
    pthread_t * threads = (pthread_t *)malloc(sizeof(pthread_t) * PTHREAD_COUNT);

    // create threads
    int rc;
    for (int i = 0; i < PTHREAD_COUNT; ++i) {
        int params[2] = { i, 3 };
        print_array(params, 2, __func__);
        printf("%s - Start thread %d\n", __func__, params[0]);
        rc = pthread_create(&threads[i], NULL, &runner_with_lock, params);
        if (rc != 0) {
            printf("%s - Failed to start thread %d, RC = %d\n",
                   __func__, params[0], rc);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < PTHREAD_COUNT; ++i) {
        printf("%s - Join thread %d\n", __func__, i);
        rc = pthread_join(threads[i], NULL);
        if (rc != 0)
            printf("%s - Failed to join thread %d, RC=%d\n", __func__, i, rc);
    }
    // clean up
    pthread_mutex_destroy(&mutex);
    free(threads);
    threads = NULL;
} /* run_threads_join_later_lock */


static void run_threads_join_immediate() {
    // define threads
    printf("%s - Define threads: runner() * %d\n", __func__, PTHREAD_COUNT);
    pthread_t * threads = (pthread_t *)malloc(sizeof(pthread_t) * PTHREAD_COUNT);

    // create threads
    int params[2], rc;
    for (int i = 0; i < PTHREAD_COUNT; ++i) {
        params[0] = i;
        params[1] = 0;
        printf("%s - Start thread %d\n", __func__, params[0]);
        rc = pthread_create(&threads[i], NULL, &runner, params);
        if (rc != 0) {
            printf("%s - Failed to start thread %d, RC = %d\n",
                   __func__, params[0], rc);
            exit(EXIT_FAILURE);
        }
        printf("%s - Join thread %d\n", __func__, i);
        pthread_join(threads[i], NULL);
    }
    // clean up
    free(threads);
    threads = NULL;
} /* run_threads_join_immediate */


int main(const int argc, const char ** argv) {
    // printf("%s\n", "*******************");
    // run_threads_join_immediate();
    // printf("%s\n", "*******************");
    // run_threads_join_later();
    printf("%s\n", "*******************");
    run_threads_join_later_lock();
    exit(EXIT_SUCCESS);
}
