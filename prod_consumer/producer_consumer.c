// -----------------------------------------------------------------------------
// Filename:    producer_consumer.c
// Revision:    $Id$
// Description: This file implements a synchronized semaphore algorithm for a
//              producer-consumer solution
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>

#include "headers/prod_cons.h"
#include "headers/queue.h"


/**
 * insert item into buffer
 *
 * @return return 0 if successful, otherwise return -1 indicating an error
 *         condition
 */
static bool insert_item(buffer_item_t item) {
    bool rc = false;

    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout;

    if (0 != sem_timedwait(&empty, &ts) && true == thread_halt) {
        printf("%s - Thread timeout, thread halt signal received.\n", __func__);
        pthread_exit(NULL);
    }
    if (0 != sem_timedwait(&mutex, &ts) && true == thread_halt) {
        printf("%s - Thread timeout, thread halt signal received.\n", __func__);
        pthread_exit(NULL);
    }
    rc = queue_offer(item);
    printf("%s - buffer after production: ", __func__);
    queue_print();

    sem_post(&mutex);
    sem_post(&full);

    return rc;
} /* insert_item */


/**
 * remove an object from buffer placing it in item
 *
 * @return return 0 if successful, otherwise return -1 indicating an error
 *         condition
 */
static bool remove_item(buffer_item_t * item) {
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout;

    if (0 != sem_timedwait(&full, &ts) && true == thread_halt) {
        printf("%s - Thread timeout, thread halt signal received.\n", __func__);
        pthread_exit(NULL);
    }
    if (0 != sem_timedwait(&mutex, &ts) && true == thread_halt) {
        printf("%s - Thread timeout, thread halt signal received.\n", __func__);
        pthread_exit(NULL);
    }

    *item = queue_poll();
    printf("%s - buffer after consumption: ", __func__);
    queue_print();

    sem_post(&mutex);
    sem_post(&empty);

    return (INT_MIN != *item);
} /* remove_item */


/**
 * Initialize the semaphores
 */
bool init_sems() {
    printf("%s - initializing semaphores\n", __func__);
    unsigned int max = BUFFER_SIZE;
    timeout = 10;
    if (0 != sem_init(&full, 0, 0) ||
        0 != sem_init(&empty, 0, max) ||
        0 != sem_init(&mutex, 0, 1)) {
        printf("%s - Error: Failed to initialize semaphores!\n", __func__);
        return false;
    }

    return true;
}


/**
 * Remove all semaphores
 */
bool destroy_sems() {
    printf("%s - destroying semaphores!\n", __func__);
    if (0 != sem_destroy(&full) ||
        0 != sem_destroy(&empty) ||
        0 != sem_destroy(&mutex)) {
        printf("%s - Error: Failed to destroy semaphores!\n", __func__);
        return false;
    }
    return true;
}


/**
 * The producer thread runner
 */
void * producer(void * params) {
    int tid = ((int *)params)[0];

    // destroy params now
    free(params);

    buffer_item_t item;

    while (false == thread_halt) {
        /* sleep for a random period of time */
        srand(time(NULL) / (tid + 1));
        // sleep 0 ~ 4 secs
        sleep(rand() % 5);
        /* generate a random number */
        srand(time(NULL) / (tid + 1));
        item = rand() % 100;
        if (false == insert_item(item))
            printf("%s - Error: Producer %d - Failed to creating product %d\n",
                   __func__, tid, item);
        else
            printf("%s - Producer %d produced %d\n", __func__, tid, item);
    }

    printf("%s - Terminating thread, thread halt signal received.\n", __func__);
    pthread_exit(NULL);
} /* producer */


/**
 * The consumer thread runner
 */
void * consumer(void * params) {
    int tid = ((int *)params)[0];

    // destroy params now
    free(params);

    buffer_item_t item;

    while (false == thread_halt) {
        /* sleep for a random period of time */
        srand(time(NULL) / (tid + 1));
        // sleep 0 ~ 4 secs
        sleep(rand() % 5);
        if (false == remove_item(&item))
            printf("%s - Error: Consumer %d - Failed to consume product\n",
                   __func__, tid);
        else
            printf("%s - Consumer %d consumed %d\n", __func__, tid, item);
    }

    printf("%s - Terminating thread, thread halt signal received.\n", __func__);
    pthread_exit(NULL);
} /* consumer */
