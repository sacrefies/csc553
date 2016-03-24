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

/**
 * Get whether the queue is initialized.
 */
bool isInitialized();


/**
 * Get whether the queue is initialized but empty.
 */
bool isEmpty();


/**
 * initialize a queue
 *
 * @param capacity  A unsigned integer which specifies the maximum capacity
 *                  of the queue.
 */
void initQueue(const unsigned int capacity);


/**
 * Clear the queue data and set the queue as an empty queue.
 */
void clearQueue();


/**
 * Destroy the queue structure and release the memory space allocated
 * to the queue.
 */
void disposeQueue();


/**
 * Add a new node at the tail of the queue.
 */
bool offer(const char * node);


/**
 * Retrieves a copy and removes the head of the queue,
 * or returns NULL if this deque is empty.
 */
char * poll();


/**
 * Get the actual size of the queue.
 */
int getLength();


/**
 * Get the maximum capacity of this queue.
 */
int getCapacity();


/**
 * Convert the queue nodes into a C string.
 */
char * toString();


/**
 * Print the converted C string which represents the queue nodes.
 */
void printQueue();

#endif /** end of include guard:  */
