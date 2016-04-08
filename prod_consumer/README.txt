------------------------------------
PROGRAM: Producer-consumer solutuion
Author : Jason Qiao Meng
ID     : 10652564
Email  : jason.meng@sju.edu
-----------------------------------

Files and Directories
---------------------
hw4
 |- headers
      |- queue.h                            // C header file for circular queue functions
      |- utils.h                            // C header file for some helper functions
      |-prod_cons.h                         // C header file for multithreading functions
 |- circular_int_queue.c                    // C circular queue implementation
 |- main.c                                  // C main program
 |- producer_consumer.c                     // C multithreading implementation
 |- utils.c                                 // C utility function implementation
 |- README.txt                              // This file


To Compile
-----------
1. Unizp the package and save the directories under a certain directory, i.e.: ~\source.
2. Launch a command line environment, change current directory to ~\source.
3. Run the below commands for the C programs:
    # gcc -std=c99 -Wall -lpthread -O3 -o prod circular_int_queue.c main.c producer_consumer.c utils.c


To Run
------
The command is as below:
  # ./prod 10 3 6


Description
-----------
Project 3: Producer - Consumer Problem

In Section 5.7.1, we presented a semaphore-based solution to the producer?
consumer problem using a bounded buffer. In this project, you will design a
programming solution to the bounded-buffer problem using the producer and
consumer processes shown in Figures 5.9 and 5.10. The solution presented in
Section 5.7.1 uses three semaphores: empty and full, which count the number of
empty and full slots in the buffer, and mutex, which is a binary (or mutual-
exclusion) semaphore that protects the actual insertion or removal of items in
the buffer. For this project, you will use standard counting semaphores for
empty and full and a mutex lock, rather than a binary semaphore, to represent
mutex. The producer and consumer - running as separate threads - will move items
to and from a buffer that is synchronized with the empty, full, and mutex
structures. You can solve this problem using either Pthreads or the Windows API.


[The Buffer]

Internally, the buffer will consist of a fixed-size array of type buffer_item
(which will be defined using a typedef). The array of buffer item objects
will be manipulated as a circular queue. The definition of buffer_item, along
with the size of the buffer, can be stored in a header file such as the
following:

    /* buffer.h */
    typedef int buffer_item;
    #define BUFFER_SIZE 5

The buffer will be manipulated with two functions, insert_item() and
remove_item(), which are called by the producer and consumer threads,
respectively. A skeleton outlining these functions appears in Figure 5.29.

    /* Figure 5.29 Outline of buffer operations. */

    #include "buffer.h"

    /* the buffer */
    buffer_item buffer[BUFFER SIZE];

    /**
     * insert item into buffer
     *
     * @return return 0 if successful, otherwise return -1 indicating an error
     *         condition
     */
    int insert_item(buffer item item) {}

    /**
     * remove an object from buffer placing it in item
     *
     * @return return 0 if successful, otherwise return -1 indicating an error
     *         condition
     */
    int remove_item(buffer item *item) {}


The insert_item() and remove_item() functions will synchronize the producer and
consumer using the algorithms outlined in Figures 5.9 and 5.10. The buffer will
also require an initialization function that initializes the mutual-exclusion
object mutex along with the empty and full semaphores.

    int n;
    semaphore mutex = 1;
    semaphore empty = n;
    semaphore full = 0;

    /* Figure 5.9 The structure of the producer process. */

    do {
        . . .
        // produce an item in next produced
        . . .
        wait(empty);
        wait(mutex);
        . . .
        // add next produced to the buffer
        . . .
        signal(mutex);
        signal(full);
    } while (true);


    /* Figure 5.10 The structure of the consumer process. */

    do {
        wait(full);
        wait(mutex);
        . . .
        // remove an item from buffer to next consumed
        . . .
        signal(mutex);
        signal(empty);
        . . .
        // consume the item in next consumed
        . . .
    } while (true);

The main() function will initialize the buffer and create the separate producer
and consumer threads. Once it has created the producer and consumer threads, the
main() function will sleep for a period of time and, upon awakening, will
terminate the application. The main() function will be passed three parameters
on the command line:

    1. How long to sleep before terminating
    2. The number of producer threads
    3. The number of consumer threads

A skeleton for this function appears in Figure 5.30.

    /* Figure 5.30 Outline of skeleton program. */

    #include "buffer.h"

    int main(int argc, char *argv[]) {
        /* 1. Get command line arguments argv[1],argv[2],argv[3] */
        /* 2. Initialize buffer */
        /* 3. Create producer thread(s) */
        /* 4. Create consumer thread(s) */
        /* 5. Sleep */
        /* 6. Exit */
    }


[The Producer and Consumer Threads]

The producer thread will alternate between sleeping for a random period of
time and inserting a random integer into the buffer. Random numbers will
be produced using the rand() function, which produces random integers
between 0 and RAND MAX. The consumer will also sleep for a random period
of time and, upon awakening, will attempt to remove an item from the buffer.
An outline of the producer and consumer threads appears in Figure 5.31.

    /* Figure 5.31 An outline of the producer and consumer threads. */

    #include <stdlib.h> /* required for rand() */
    #include "buffer.h"

    void *producer(void *param) {
        buffer_item item;

        while (true) {
            /* sleep for a random period of time */
            sleep(...);
            /* generate a random number */
            item = rand();
            if (insert_item(item))
                printf("report error condition");
            else
                printf("producer produced %d\n", item);
        }
    }

    void *consumer(void *param) {
        buffer_item item;

        while (true) {
            /* sleep for a random period of time */
            sleep(...);
            if (remove_item(&item))
                printf("report error condition");
            else
                printf("consumer consumed %d\n", item);
        }
    }

As noted earlier, you can solve this problem using either Pthreads or the
Windows API. In the following sections, we supply more information on each
of these choices.


[Pthreads Thread Creation and Synchronization]

Creating threads using the Pthreads API is discussed in Section 4.4.1. Coverage
of mutex locks and semaphores using Pthreads is provided in Section 5.9.4.
Refer to those sections for specific instructions on Pthreads thread creation and
synchronization.
