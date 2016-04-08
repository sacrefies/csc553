// -----------------------------------------------------------------------------
// Filename:    circular_int_queue.cpp
// Revision:    $Id$
// Description: This file includes the implementation of the circular queue.
//              This queue consists of nodes of C integer. This queue internally
//              holds an array of int as the data storage.
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
#include <limits.h>

#include "headers/utils.h"
#include "headers/queue.h"


/** internal buffer to store the queue items */
static int * q_buff = NULL;
/** queue's head and tail */
static int front_index, tail_index;
/** the queue's actual size (length) and its initial capacity */
static unsigned int size, cap;


/**
 * Calculate and return the next index for adding a new node into the queue.
 */
static int next_index(const int curr_index) {
    if (curr_index == -1)
        return 0;
    return (curr_index + 1) % cap;
}


/**
 * Check if the queue's size has reached the current capacity
 *
 * @return Returns true if size == capacity; returns false otherwise.
 */
static bool is_full() {
    return size == cap;
}


/**
 * Increase internal buffer size to the next legal capacity when the actual size
 * meets the current capacity.
 */
static void grow() {
    int old_cap = cap;

    cap = (int)(cap * (1 + GROWTH_FACTOR));
    // create the new inner buffer with the new capacity
    int * new_queue = (int *)malloc(sizeof(int) * cap);
    // copy the current inner buffer to the new inner buffer
    // starting from front to tail
    for (int i = 0; i < size; ++i) {
        new_queue[i] = q_buff[(i + front_index) % old_cap];
    }
    // destroy old buffer
    free(q_buff);
    // replace
    q_buff = new_queue;
    // reset front, tail
    front_index = 0;
    tail_index = size - 1;
}


/**
 * Get whether the queue is initialized.
 */
bool queue_initialized() {
    return q_buff != NULL;
}


/**
 * Get whether the queue is initialized but empty.
 */
bool queue_is_empty() {
    return (size == 0 && queue_initialized());
}


/**
 * initialize a queue
 *
 * @param capacity  A unsigned integer which specifies the maximum capacity
 *                  of the queue.
 */
void queue(const unsigned int capacity) {
    if (true == queue_initialized())
        return;

    cap = capacity;
    if (cap == 0)
        cap = CAPACITY;
    q_buff = (int *)malloc(sizeof(int) * cap);
    size = 0;
    tail_index = front_index = -1;
}


/**
 * Clear the queue data and set the queue as an empty queue.
 */
void queue_clear() {
    if (false == queue_initialized())
        return;
    if (true == queue_is_empty())
        return;

    tail_index = front_index = -1;
    size = 0;
}


/**
 * Destroy the queue structure and release the memory space allocated
 * to the queue.
 */
void queue_dispose() {
    if (false == queue_initialized())
        return;

    free(q_buff);
    q_buff = NULL;
    front_index = -1;
    tail_index = -1;
    size = -1;
}


/**
 * Add a new node at the tail of the queue.
 */
bool queue_offer(const int data) {
    if (false == queue_initialized())
        return false;

    if (true == is_full()) {
        // grow the buffer
        grow();
    }

    tail_index = next_index(tail_index);
    // enqueue the data
    q_buff[tail_index] = data;
    // set new front, tail
    if (front_index == -1) front_index = 0;
    size++;
    return true;
}


/**
 * Get the current front without removing it.
 *
 * @return The data stored in the front; or INT_MIN if any exception happens.
 */
int queue_peek() {
    if (false == queue_initialized() || true == queue_is_empty())
        return INT_MIN;

    return q_buff[front_index];
}


/**
 * Retrieves a copy and removes the head of the queue
 *
 * @return The data stored in the front; or INT_MIN if any exception happens.
 */
int queue_poll() {
    if (false == queue_initialized() || true == queue_is_empty())
        return INT_MIN;

    int data = q_buff[front_index];
    front_index = next_index(front_index);
    size--;
    if (true == queue_is_empty())
        front_index = tail_index = -1;
    return data;
}


/**
 * Return data from an element at the specified position.
 *
 * @param pos   Index of the element.
 */
int queue_element(const int pos) {
    if (false == queue_initialized() || true == queue_is_empty() ||
        pos < 0 || pos >= size) {
        return INT_MIN;
    }
    int internal_pos = (pos + front_index) % cap;
    return q_buff[internal_pos];
}


/**
 * Get the actual size of the queue.
 */
int queue_length() {
    return size;
}


/**
 * Get the maximum capacity of this queue.
 */
int queue_capacity() {
    return cap;
}


/**
 * Convert the queue nodes into a C string.
 */
char * queue_to_string() {
    char * out = NULL;

    if (false == queue_initialized() || true == queue_is_empty()) {
        // form a string: '[]\0' -> size == 3
        out = (char *)malloc(sizeof(char));
        out[0] = '\0';
        return out;
    }

    int out_size = 0;
    char ** ptr = (char **)malloc(sizeof(char *) * size);

    if (front_index <= tail_index) {
        for (int i = front_index; i <= tail_index; ++i) {
            ptr[i - front_index] = int_to_string(q_buff[i]);
            out_size += (int)strlen(ptr[i - front_index]);
        }
    } else {
        for (int i = front_index; i <= (tail_index + cap); ++i) {
            int index = i % cap;
            ptr[i - front_index] = int_to_string(q_buff[index]);
            out_size += strlen(ptr[i - front_index]);
        }
    }

    out = (char *)malloc(sizeof(char) * (out_size + size));
    out[0] = '\0';
    for (int i = 0; i < size; ++i) {
        strncat(out, ptr[i], strlen(ptr[i]));
        if (i < size - 1)
            strncat(out, ",", 1);
    }

    // clean up
    for (int i = 0; i < size; ++i) {
        free(ptr[i]);
        ptr[i] = NULL;
    }
    free(ptr);
    ptr = NULL;

    return out;
} /* queue_to_string */


/**
 * Print the converted C string which represents the queue nodes.
 */
void queue_print() {
    char * out = queue_to_string();

    printf("%s: [%s]\n", __func__, out);
    free(out);
    out = NULL;
}


// /**
//  * Unit tests for the circular queue implementation
//  * Run: # circularqueue
//  *      # circularqueue
//  */
// int main(const int argc, const char * argv[]) {
//     int capacity = 5;
//
//     printf("\n**** Test: non-initialized queue\n");
//     bool empty = queue_is_empty();
//     bool inited = queue_initialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'false\' actual \'%s\'\n", inited ? "true" : "false");
//     queue_print();
//
//     printf("\n**** Test: initialized empty queue\n");
//     queue(capacity);
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     queue_print();
//
//     printf("\n**** Test: 1 node queue\n");
//     queue_offer(1);
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     int size = queue_length();
//     int node = queue_peek();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Peek: Expected 1, actual %d\n", node);
//     printf("Size: Expected %d, actual %d\n", 1, size);
//     queue_print();
//
//     printf("\n**** Test: Adding 4 nodes\n");
//     queue_offer(2);
//     queue_offer(3);
//     queue_offer(4);
//     queue_offer(5);
//     size = queue_length();
//     int capc = queue_capacity();
//     node = queue_peek();
//     printf("Peek: Expected 1, actual %d\n", node);
//     printf("Size: Expected %d, actual %d\n", capacity, size);
//     printf("Cap: Expected %d, actual %d\n", capacity, capc);
//     queue_print();
//
//     printf("\n**** Test: Elements of a 5 node queue\n");
//     for (int i = 0; i < 5; ++i) {
//         node = queue_element(i);
//         printf("Elem: Expected %d, actual %d\n", i + 1, node);
//     }
//     node = queue_peek();
//     size = queue_length();
//     capc = queue_capacity();
//     printf("Peek: Expected 1, actual %d\n", node);
//     printf("Size: Expected %d, actual %d\n", capacity, size);
//     printf("Cap: Expected %d, actual %d\n", capacity, capc);
//
//     printf("\n**** Test: Polling the heads\n");
//     node = queue_poll();
//     printf("Poll: Expected \'1\', actual %d\n", node);
//     node = queue_poll();
//     printf("Poll: Expected \'2\', actual %d\n", node);
//     size = queue_length();
//     capc = queue_capacity();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     node = queue_peek();
//     printf("Peek: Expected 3, actual %d\n", node);
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", capacity - 2, size);
//     printf("Cap: Expected %d, actual %d\n", capacity, capc);
//     queue_print();
//
//     printf("\n**** Test: Exceed the capacity and grow\n");
//     queue_offer(6);
//     queue_offer(7);
//     queue_offer(8);
//     queue_offer(9);
//     queue_offer(10);
//     size = queue_length();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     node = queue_peek();
//     capc = queue_capacity();
//     printf("Peek: Expected 3, actual %d\n", node);
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 8, size);
//     printf("Cap: Expected = %d, actual %d\n", (int)(capacity * (1 + GROWTH_FACTOR)), capc);
//     queue_print();
//
//     printf("\n**** Test: toString()\n");
//     char * out = queue_to_string();
//     printf("toString: %s\n", out);
//     free(out);
//
//     printf("\n**** Test: Polling all\n");
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     printf("Poll when empty - ");
//     node = queue_poll();
//     printf("Poll: %d\n", node);
//     size = queue_length();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     capc = queue_capacity();
//     node = queue_peek();
//     printf("Peek: Expected INT_MIN, actual %d\n", node);
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 0, size);
//     printf("Cap: Expected = %d, actual %d\n", (int)(capacity * (1 + GROWTH_FACTOR)), capc);
//     queue_print();
//
//     printf("\n**** Test: Adding extra 4 nodes\n");
//     queue_offer(-1);
//     queue_offer(-2);
//     queue_offer(-3);
//     queue_offer(-4);
//     size = queue_length();
//     capc = queue_capacity();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     node = queue_peek();
//     printf("Peek: Expected -1, actual %d\n", node);
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 4, size);
//     printf("Cap: Expected = %d, actual %d\n", (int)(capacity * (1 + GROWTH_FACTOR)), capc);
//     queue_print();
//
//     printf("\n**** Test: toString()\n");
//     out = queue_to_string();
//     printf("toString: %s\n", out);
//     free(out);
//
//     printf("\n**** Test: clear()\n");
//     queue_clear();
//     size = queue_length();
//     capc = queue_capacity();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     node = queue_peek();
//     printf("Peek: Expected INT_MIN, actual %d\n", node);
//     printf("Empty: Expected \'true\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'true\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", 0, size);
//     printf("Cap: Expected = %d, actual %d\n", (int)(capacity * (1 + GROWTH_FACTOR)), capc);
//     queue_print();
//
//     printf("\n**** Test: Destroy the queue\n");
//     // destroy queue
//     queue_dispose();
//     size = queue_length();
//     capc = queue_capacity();
//     empty = queue_is_empty();
//     inited = queue_initialized();
//     printf("Empty: Expected \'false\', actual \'%s\'\n", empty ? "true" : "false");
//     printf("Init: Expected \'false\' actual \'%s\'\n", inited ? "true" : "false");
//     printf("Size: Expected %d, actual %d\n", -1, size);
//     printf("Cap: Expected = %d, actual %d\n", (int)(capacity * (1 + GROWTH_FACTOR)), capc);
//
//     out = NULL;
// } /* main */
