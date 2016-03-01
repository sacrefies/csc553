// -----------------------------------------------------------------------------
//       Filename: forks.c
//       Revision: $Id$
//    Description: This file includes a study program to demo process and child
//                 processes.
//        Created: 02/15/2016 01:20:29 PM
//       Compiler: GCC
//         Author: Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016 by Jason Meng, no rights reserved.
// -----------------------------------------------------------------------------


#include <stdio.h>
#include <unistd.h>

/** this program prints 5 hello */
int main(int argc, char * argv[]) {
    if (fork() == 0) {
        printf("child1 hello\n");
    } else if (fork() == 0) {
        printf("child2 hello\n");
    }
    printf("hello\n");
}
