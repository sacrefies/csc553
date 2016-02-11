// ------------------------------------------------------------------------------------
//            $Id$
//          $Date$
//       Filename:  pid.c
//
//    Description:  
//
//        Version:  1.0
//        Created:  02/11/2016 05:05:27 AM
//       Compiler:  g++
//         Author:  Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016, Jason Meng
// ------------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>


int main()
{
    pid_t pid, pid1;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0) { /* child process */
        pid1 = getpid();
        printf("child: pid = %d\n",pid); /* A */
        printf("child: pid1 = %d\n",pid1); /* B */
    } else { /* parent process */
        pid1 = getpid();
        wait(NULL);
        printf("parent: pid = %d\n",pid); /* C */
        printf("parent: pid1 = %d\n",pid1); /* D */
    }
    return 0;
}

