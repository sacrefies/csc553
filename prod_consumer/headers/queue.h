// -----------------------------------------------------------------------------
// Filename:    queue.h
// Revision:    $Id$
// Description: This header file defines the standard functions to interact with
//              a queue.
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


/** include guard:  */
#ifndef QUEUE_H
#define QUEUE_H


#define CAPACITY      8
#define GROWTH_FACTOR 0.70


/**
 * Get whether the queue is initialized.
 */
bool queue_initialized();


/**
 * Get whether the queue is initialized but empty.
 */
bool queue_is_empty();


/**
 * initialize a queue
 *
 * @param capacity  A unsigned integer which specifies the maximum capacity
 *                  of the queue.
 */
void queue(const unsigned int capacity);

/**
 * Clear the queue data and set the queue as an empty queue.
 */
void queue_clear();


/**
 * Destroy the queue structure and release the memory space allocated
 * to the queue.
 */
void queue_dispose();


/**
 * Add a new node at the tail of the queue.
 */
bool queue_offer(const int data);


/**
 * Get the current front without removing it.
 *
 * @return The data stored in the front; or INT_MIN if any exception happens.
 */
int queue_peek();


/**
 * Retrieves a copy and removes the head of the queue
 *
 * @return The data stored in the front; or INT_MIN if any exception happens.
 */
int queue_poll();


/**
 * Return data from an element at the specified position.
 *
 * @param pos   Index of the element.
 */
int queue_element(const int pos);


/**
 * Get the actual size of the queue.
 */
int queue_length();


/**
 * Get the maximum capacity of this queue.
 */
int queue_capacity();


/**
 * Convert the queue nodes into a C string.
 */
char * queue_to_string();


/**
 * Print the converted C string which represents the queue nodes.
 */
void queue_print();

#endif /** end of include guard:  */
