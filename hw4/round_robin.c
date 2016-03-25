// -----------------------------------------------------------------------------
// Filename:    round_robin.c
// Revision:    $Id$
// Description: This file implements an emulation of round-robin process scheduler
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "headers/queue.h"
#include "headers/utils.h"


#ifndef ARG_COUNT
#define ARG_COUNT 7
#endif

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int * burstTimes = NULL;
bool * ready = NULL;
int quantum = 0;


/**
 * Print the help message
 */
static void printUsage(const char * prog) {
    printf("%s <quantum> <P1_burst_time ... P5_burst_time>\n", prog);
    // printf("Or\n");
    // printf("%s <process_count> <quantum> <P1_burst_time ... Pn_burst_time>\n", prog);
    printf("Examples:\n");
    // printf("$ %s 3 2 10 13 5\n", prog);
    // printf("Generate 5 processes by omitting the <process_count> argument\n");
    printf("$ %s 2 10 13 5 7 1\n", prog);
}


/**
 * Validate a specified command line argument.
 *
 * @param arg   A command line argument.
 *
 * @return  Returns true if the argument is valid; returns false if otherwise.
 */
static bool validateArg(const char * arg) {
    // is it a valid number?
    if (false == isNumber(arg)) {
        printf("Not a number: %s\n", arg);
        return false;
    }
    if (toInt(arg) <= 0) {
        printf("Not a positive number: %s\n", arg);
        return false;
    }
    return true;
}


/**
 * Check and validate all arguments
 */
static void checkArgsSanity(const int argc, const char * argv[]) {
    // argument count at least >= 3
    if (argc < ARG_COUNT) {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i < ARG_COUNT; ++i) {
        if (false == validateArg(argv[i]))
            exit(EXIT_FAILURE);
    }
}


/**
 * Thread function which simulates a normal thread run.
 */
static void * threadRun(void * params) {
    int procIndex = ((int *)params)[0];

    while (burstTimes[procIndex] > 0) {
        pthread_mutex_lock(&mutex);
        // if not ready, keep waiting and release the mutex
        while (false == ready[procIndex])
            pthread_cond_wait(&cond, &mutex);
        // ready, go
        // run for runTime duration
        int runTime = (burstTimes[procIndex] >= quantum) ?
            quantum : burstTimes[procIndex];
        printf("P%d - %d seconds\n", procIndex, runTime);
        sleep(runTime);
        // minus burst time by the runTime
        burstTimes[procIndex] -= runTime;
        // set ready flag = false, let the scheduler make decision
        ready[procIndex] = false;
        // notify others ready flag has been changed
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    printf("P%d - =====> Exit\n", procIndex);
    pthread_exit(NULL);
} /* threadRun */


/**
 * A scheduler thread for thread management which implements round-robin
 */
static void * scheduler(void * params) {
    char buff[10];

    // create a queue for the process identities
    initQueue(5);
    for (int i = 0; i < (ARG_COUNT - 2); ++i) {
        snprintf(buff, 10, "%d", i);
        offer(buff);
    }
    // poll head
    // set ready flag
    // wake up thread
    // check remaining burst time
    // offer again
    char * proc = NULL;
    while (false == isEmpty()) {
        // Lock mutex and then wait for signal to relase mutex
        pthread_mutex_lock(&mutex);
        proc = poll();
        // printf("Poll: %s\n", proc);
        int procIndex = toInt(proc);
        free(proc);

        if (burstTimes[procIndex] > 0) {
            ready[procIndex] = true;
            snprintf(buff, 10, "%d", procIndex);
            offer(buff);
            // notify all other threads => ready flags have been changed
            pthread_cond_broadcast(&cond);
            // wait for one of other threads notification
            pthread_cond_wait(&cond, &mutex);
        }
        // unlock mutex and signal
        pthread_mutex_unlock(&mutex);
    }
    proc = NULL;
    disposeQueue();

    pthread_exit(NULL);
} /* scheduler */


int main(const int argc, const char * argv[]) {

    // check the sanity of the arguments
    checkArgsSanity(argc, argv);

    // init critical conditions for scheduler
    burstTimes = (int *)malloc(sizeof(int) * (ARG_COUNT - 2));
    ready = (bool *)malloc(sizeof(bool) * (ARG_COUNT - 2));

    quantum = toInt(argv[1]);
    // (ACCEPT_ARG_COUNT - 2) normal running thread
    // and +1 scheduler thread.
    pthread_t * threads = (pthread_t *)malloc(sizeof(pthread_t) * (ARG_COUNT - 1));
    // set up the burst times
    // args that appear at position behind ACCEPT_ARG_COUNT
    // will be omitted.
    for (int i = 0; i < ARG_COUNT - 2; ++i) {
        burstTimes[i] = toInt(argv[i + 2]);
        // set all ready flag false
        ready[i] = false;
    }

    // create threads, join the threads
    int rc;
    for (int tid = 0; tid < ARG_COUNT - 1; ++tid) {
        // create normal threads before the scheduler thread.
        // they will keep waiting for the scheduler do dispatching job
        if (tid < ARG_COUNT - 2) {
            int * params = (int *)malloc(sizeof(int));
            params[0] = tid;
            rc = pthread_create(&threads[tid], NULL, &threadRun, (void *)params);
        } else {
            // the scheduler thread
            rc = pthread_create(&threads[tid], NULL, &scheduler, NULL);
            // join only the scheduler is enough
            pthread_join(threads[tid], NULL);
        }
        if (rc != 0) {
            printf("Creating thread %d failed. RC = %d\n", tid, rc);
            exit(EXIT_FAILURE);
        }
    }

    // clean up
    free(burstTimes);
    free(ready);
    free(threads);

    burstTimes = NULL;
    ready = NULL;
    threads = NULL;

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    exit(0);
} /* main */
