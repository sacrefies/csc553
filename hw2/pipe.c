// ------------------------------------------------------------------------------------
//            $Id$
//          $Date$
//       Filename:  pipe.cpp
//
//    Description:  This program simulates Linux pipe mechanism.
//                  It produces the result of the below commands:
//                  ls -l | sort | grep .c
//
//        Version:  1.0
//        Created:  02/10/2016 11:35:15 PM
//       Compiler:  GCC
//         Author:  Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016, Jason Meng
// ------------------------------------------------------------------------------------

// c includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// pipe read
#define RD 0
// pipe write
#define WT 1

int main(int argc, char *argv[]) {
    // define pipe 1
    int p1[2];
    // define pipe 2
    int p2[2];
    // create pipe 1
    // this p1 is only for parent-first_child
    pipe(p1);
    pipe(p2);

    switch (fork()) {
        case -1:
            perror("Fork error");
            break;
        case 0:
            // pid = 0, child process
            // The child process does ls part
            close(1);
            // replace stdout with p1[1]
            dup(p1[WT]);
            // close not used pipes
            close(p1[RD]);
            close(p1[WT]);
            close(p2[RD]);
            close(p2[WT]);

            execlp("ls", "ls", "-alh", NULL);
            break;
        default:
            switch (fork()) {
                case -1:
                    perror("Fork error");
                    break;
                case 0:
                    // the second child of the parent
                    // this child does the sort part

                    // no need for std in/out
                    close(0);
                    close(1);

                    // replace std in/out with p2
                    dup(p1[RD]);
                    dup(p2[WT]);

                    close(p1[RD]);
                    close(p1[WT]);
                    close(p2[RD]);
                    close(p2[WT]);

                    execlp("sort", "sort", NULL);
                    break;
                default:
                    // the parent which does the grep part
                    close(0);   // no need for stdin here
                    dup(p2[RD]);

                    close(p1[RD]);
                    close(p1[WT]);
                    close(p2[RD]);
                    close(p2[WT]);

                    execlp("grep", "grep", ".c", NULL);
                    break;
            }
    }
    return 0;
}

