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
    int * args = (int *)params;
    int procIndex = (int)args[0];

    printf("Proc %d - Received params[0] = %d\n", procIndex, ((int *)params)[0]);

    printf("Proc %d - Running\n", procIndex);
    // Note: don't do critical condition test outside the locking area
    while (burstTimes[procIndex] > 0) {
        printf("Proc %d - Acquiring lock\n", procIndex);
        pthread_mutex_lock(&mutex);
        // if not ready, keep waiting and release the mutex
        printf("Proc %d - ready = %s\n", procIndex,
               (ready[procIndex] == true) ? "true" : "false");

        if (false == ready[procIndex]) {
            printf("Proc %d - Wait\n", procIndex);
            pthread_cond_wait(&cond, &mutex);
        } else {
            // ready, go
            // run for runTime duration
            int runTime = (burstTimes[procIndex] >= quantum) ?
                quantum : burstTimes[procIndex];
            // minus burst time by the runTime
            burstTimes[procIndex] -= runTime;
            printf("Proc %d - Calc run time: %d\n", procIndex, runTime);
            printf("Proc %d - Remaining run time: %d\n", procIndex, burstTimes[procIndex]);
            printf("Proc %d - Running\n", procIndex);
            sleep(runTime);
            printf("Proc %d - Set ready flag to false\n", procIndex);
            ready[procIndex] = false;
            printf("Proc %d - Releasing lock\n", procIndex);
        }
        pthread_mutex_unlock(&mutex);
    }
    printf("Proc %d - Run time exceeded, exit\n", procIndex);
    printf("Proc %d - =====> Exit\n", procIndex);
    pthread_exit(NULL);
} /* threadRun */


/**
 * A scheduler thread for thread management which implements round-robin
 */
static void * scheduler(void * params) {
    char buff[10];

    // create a queue for the process identities
    printf("Scheduler - Init process queue: ");
    initQueue(5);
    for (int i = 0; i < (ARG_COUNT - 2); ++i) {
        snprintf(buff, 10, "%d", i);
        offer(buff);
    }
    printQueue();

    // poll head
    // set ready flag
    // wake up thread
    // check remaining burst time
    // offer again
    char * proc = NULL;
    int procIndex = 0;
    while (false == isEmpty()) {
        printf("Scheduler - Acquiring lock\n");
        // Lock mutex and then wait for signal to relase mutex
        pthread_mutex_lock(&mutex);
        proc = poll();
        procIndex = toInt(proc);
        printf("Scheduler - Polled from ready queue proc: %s\n", proc);
        free(proc);
        proc = toString();
        printf("Scheduler - Ready queue after poll: %s\n", proc);
        free(proc);

        printf("Scheduler - Proc %d remaining burst time: %d\n",
               procIndex, burstTimes[procIndex]);
        if (burstTimes[procIndex] > 0) {
            printf("Scheduler - Proc %d set ready\n", procIndex);
            ready[procIndex] = true;
            if (burstTimes[procIndex] > 0) {
                printf("Scheduler - Proc %d remaining time next: %d\n",
                       procIndex, burstTimes[procIndex] - quantum);
                snprintf(buff, 10, "%d", procIndex);
                printf("Scheduler - Add proc %d to ready queue\n", procIndex);
                offer(buff);
                proc = toString();
                printf("Scheduler - Ready queue after offer: %s\n", proc);
                free(proc);

                printf("Scheduler - Current burst times: ");
                printArray(ARG_COUNT - 2, burstTimes, "");
                printf("Scheduler - Current ready flags: ");
                printArray(ARG_COUNT - 2, (int *)ready, "");
            }
            pthread_cond_broadcast(&cond);
        }
        // unlock mutex and signal
        printf("Scheduler - Releasing lock\n");
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
    pthread_t threads[ARG_COUNT - 1];

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
            int params[1] = { tid };
            printf("Creating thread %d, params[0]=%d\n", tid, params[0]);
            rc = pthread_create(&threads[tid], NULL, &threadRun, (void *)params);
            // pthread_create(&threads[tid], NULL, &threadRun, (void *)params);
        } else {
            printf("Creating scheduler thread tid = %d\n", tid);
            rc = pthread_create(&threads[tid], NULL, &scheduler, NULL);
        }
        if (rc != 0) {
            printf("Creating thread %d failed. RC = %d\n", tid, rc);
            exit(EXIT_FAILURE);
        }

        //pthread_join(threads[tid], NULL);
    }

    for (int tid = 0; tid < ARG_COUNT - 1; ++tid) {
        printf("Joining thread %d\n", tid);
        pthread_join(threads[tid], NULL);
    }

    // clean up
    free(burstTimes);
    free(ready);
    // free(threads);

    burstTimes = NULL;
    ready = NULL;
    // threads = NULL;

    exit(0);
} /* main */
