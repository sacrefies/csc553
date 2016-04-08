// -----------------------------------------------------------------------------
// Filename:    prod_cons.h
// Revision:    $Id$
// Description: This header file defines the functions of producer-consumer
//              solver
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <semaphore.h>
#include <stdbool.h>


#ifndef PROD_CONS_H
#define PROD_CONS_H

#define BUFFER_SIZE 10
typedef int buffer_item_t;

sem_t full;
sem_t empty;
sem_t mutex;

bool thread_halt;
int timeout;


/**
 * Initialize the semaphores
 */
bool init_sems();


/**
 * Remove all semaphores
 */
bool destroy_sems();


/**
 * The producer thread runner
 */
void * producer(void * params);


/**
 * The consumer thread runner
 */
void * consumer(void * params);

#endif /* ifndef PROD_CONS_H */
