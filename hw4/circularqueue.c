// -----------------------------------------------------------------------------
// Filename:    circularqueue.cpp
// Revision:    $Id$
// Description: This file includes the implementation of the circular queue.
//              This queue consists of nodes of C string.
//              This queue internally holds an array of string as the data storage.
// Created:     03/21/2016 01:20:29 PM
// Compiler:    GCC
// Author:      Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "headers/queue.h"

#define CAPACITY        10
#define QUEUE_NODE_SIZE 10


static char ** q_buff = NULL;
static int front_index, tail_index;
static int size, cap;

static int getNextIndex(const int curr_index) {
    if (curr_index == -1)
        return 0;
    return (curr_index + 1) % cap;
}


static bool isFull() {
    return size == cap;
}


/**
 * Copy string src to dest.
 */
static void copyString(char * dest, const char * src) {
    int dest_size = sizeof(dest);
    bool null_ending_needed = (dest_size <= (strlen(src) + 1));

    strncpy(dest, src, dest_size);
    if (true == null_ending_needed)
        dest[dest_size - 1] = '\0';
}


/**
 * Get whether the queue is initialized.
 */
bool isInitialized() {
    return q_buff != NULL;
}


/**
 * Get whether the queue is initialized but empty.
 */
bool isEmpty() {
    return (size == 0 && isInitialized());
}


/**
 * initialize a queue
 *
 * @param capacity  A unsigned integer which specifies the maximum capacity
 *                  of the queue.
 */
void initQueue(const unsigned int capacity) {
    cap = capacity;
    if (cap == 0)
        cap = CAPACITY;
    q_buff = (char **)malloc(sizeof(char *) * cap);
    for (unsigned int i = 0; i < cap; ++i) {
        q_buff[i] = (char *)malloc(sizeof(char) * (QUEUE_NODE_SIZE + 1));
    }

    size = 0;
    tail_index = front_index = -1;
}


/**
 * Clear the queue data and set the queue as an empty queue.
 */
void clearQueue() {
    tail_index = front_index = -1;
}


/**
 * Destroy the queue structure and release the memory space allocated
 * to the queue.
 */
void disposeQueue() {
    for (unsigned int i = 0; i < cap; ++i) {
        free(q_buff[i]);
        q_buff[i] = NULL;
    }

    free(q_buff);
    q_buff = NULL;
    front_index = -1;
    tail_index = -1;
    size = -1;
}


/**
 * Add a new node at the tail of the queue.
 */
bool offer(const char * node) {
    if (true == isFull())
        return false;

    int next = getNextIndex(tail_index);
    // enqueue the data
    copyString(q_buff[next], node);
    // set new front, tail
    if (front_index == -1) front_index = 0;
    tail_index = next;
    size++;
    return true;
}


/**
 * Retrieves a copy and removes the head of the queue,
 * or returns NULL if this deque is empty.
 */
char * poll() {
    if (true == isEmpty())
        return NULL;

    int next = getNextIndex(front_index);
    char * node = (char *)malloc(sizeof(char) * (strlen(q_buff[front_index]) + 1));
    copyString(node, q_buff[front_index]);
    front_index = next;
    size--;
    if (true == isEmpty())
        front_index = tail_index = -1;
    return node;
}


/**
 * Get the actual size of the queue.
 */
int getLength() {
    return size;
}


/**
 * Get the maximum capacity of this queue.
 */
int getCapacity() {
    return cap;
}


/**
 * Convert the queue nodes into a C string.
 */
char * toString() {
    char * out = NULL;

    if (true == isEmpty() || false == isInitialized()) {
        // form a string: '[]\0' -> size == 3
        out = (char *)malloc(sizeof(char) * 3);
        copyString(out, "");
    } else {
        int out_size = sizeof(char) * (size * (QUEUE_NODE_SIZE + 2) - 1);
        out  = (char *)malloc(out_size);
        copyString(out, "");
        if (front_index <= tail_index) {
            for (int i = front_index; i <= tail_index; ++i) {
                strncat(out, q_buff[i], strlen(q_buff[i]));
                if (i != tail_index)
                    strncat(out, ",", 1);
            }
        } else {
            for (int i = front_index; i <= (tail_index + cap); ++i) {
                int index = i % cap;
                strncat(out, q_buff[index], strlen(q_buff[index]));
                if (index != tail_index)
                    strncat(out, ",", 1);
            }
        }
    }
    return out;
} /* toString */


/**
 * Print the converted C string which represents the queue nodes.
 */
void printQueue() {
    printf("Print queue: [%s]\n", toString());
} /* printQueue */


// /**
//  * Unit tests for the circular queue implementation
//  * Run: # circularqueue
//  *      # circularqueue
//  */
// int main(const int argc, const char * argv[]) {
//     int capacity = CAPACITY;
//
//     printf("\n**** Test: non-initialized queue\n");
//     bool empty = isEmpty();
//     bool inited = isInitialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'false\' actual \'%s\'\n", inited ? "true" : "false");
//     printQueue();
//
//     printf("\n**** Test: initialized empty queue\n");
//     initQueue(capacity);
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printQueue();
//
//     printf("\n**** Test: 1 node queue\n");
//     offer("P1");
//     empty = isEmpty();
//     inited = isInitialized();
//     int size = getLength();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 1, size);
//     printQueue();
//
//     printf("\n**** Test: Adding 4 nodes\n");
//     offer("P2");
//     offer("P3");
//     offer("P4");
//     offer("P5");
//     size = getLength();
//     printf("Size: Expected %d, actual %d\n", 5, size);
//     printQueue();
//
//     printf("\n**** Test: Polling the heads\n");
//     char * node = poll();
//     printf("Poll: Expected \'P1\', actual \'%s\'\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: Expected \'P2\', actual \'%s\'\n", node);
//     free(node);
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 3, size);
//     printQueue();
//
//     printf("\n**** Test: Adding to maximum capacity\n");
//     offer("P6");
//     offer("P7");
//     offer("P8");
//     offer("P9");
//     offer("P10");
//     offer("P11");
//     offer("P12");
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 10, size);
//     printf("Capacity = %d\n", capacity);
//
//     printf("\n**** Test: Overflow adding is not allowed\n");
//     bool succ = offer("P13");
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Offer: Expected \'false\', actual \'%s\'\n", succ ? "true" : "false");
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 10, size);
//     printf("Capacity = %d\n", capacity);
//     printQueue();
//
//     printf("\n**** Test: toString()\n");
//     char * out = toString();
//     printf("toString: %s\n", out);
//     free(out);
//
//     printf("\n**** Test: Polling all\n");
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     node = poll();
//     printf("Poll: %s\n", node);
//     free(node);
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 0, size);
//     printQueue();
//
//     printf("\n**** Test: Polling empty queue gets NULL\n");
//     node = poll();
//     succ = (node == NULL);
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Result: Expected NULL, actual %s", succ ? "NULL" : "NOT NULL");
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 0, size);
//
//     printf("\n**** Test: Adding 4 nodes\n");
//     offer("P2");
//     offer("P3");
//     offer("P4");
//     offer("P5");
//     size = getLength();
//     printf("Size: Expected %d, actual %d\n", 4, size);
//     printQueue();
//
//     printf("\n**** Test: toString()\n");
//     out = toString();
//     printf("toString: %s\n", out);
//     free(out);
//     printf("\n**** Test: Destroy the queue\n");
//
//     // destroy queue
//     disposeQueue();
//     size = getLength();
//     empty = isEmpty();
//     inited = isInitialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'false\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", -1, size);
//
//     // clean up
//     node = NULL;
//     out = NULL;
// } /* main */
