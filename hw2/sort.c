// ------------------------------------------------------------------------------------
//            $Id$
//          $Date$
//       Filename:  sort.c
//
//    Description:  This program demonstrates the process control of Linux.
//                  It creates 2 process where the parent process sorts array
//                  elements in descending order and prints them out and
//                  child process sorts array elements in ascending order
//                  and prints them out.
//
//        Version:  1.0
//        Created:  02/11/2016 02:20:11 AM
//       Compiler:  g++
//         Author:  Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016, Jason Meng
// ------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>


void fill_array(int size, int *array) {
    if (size == 0)
        return;

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % size;
    }
}


void print_array(int size, int *array) {
    printf("%s -- [", __func__);
    for (int i = 0; i < size; ++i)
        printf("%d,", array[i]);
    printf("]\n");
}


void sort_asc(int size) {
    int *array = NULL;
    array = (int*)malloc(sizeof(int) * size);

    fill_array(size, array);

    // init tmp
    int sortedIndex, key;
    sortedIndex = key = 0;
    for (int i = 1; i < size; i++) {
        // get the key
        key = array[i];
        // find a space for the key in the sorted sublist.
        for (sortedIndex = i - 1; sortedIndex >= 0; sortedIndex--) {
            if (array[sortedIndex] > key)
                array[sortedIndex+1] = array[sortedIndex];
            else
                break;
        }
        // insert the key here
        array[sortedIndex+1] = key;
    }

    print_array(size, array);

    free(array);
}


void sort_desc(int size) {
    int *array = NULL;
    array = (int*)malloc(sizeof(int) * size);

    fill_array(size, array);

    int sortedIndex, key;
    sortedIndex = key = 0;
    for (int i = 1; i < size; i++) {
        key = array[i];
        for (sortedIndex = i - 1; sortedIndex >= 0; sortedIndex--) {
            if (array[sortedIndex] < key) {
                array[sortedIndex+1] = array[sortedIndex];
            } else {
                break;
            };
        };
        array[sortedIndex+1] = key;
    };

    print_array(size, array);

    free(array);
}



int main(int argc, char *argv[]) {
    int status;
    switch (fork()) {
        case -1:
            perror("Fork error");
            return -1;
        case 0: // child process
            printf("The child sort in ascending order:\n");
            sort_asc(10);
            break;
        default: // parent process
            wait(&status);
            printf("The parent sort in descending order:\n");
            sort_desc(10);
            break;
    }

    return 0;

}
