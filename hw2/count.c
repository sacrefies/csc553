// ------------------------------------------------------------------------------------
//            $Id$
//          $Date$
//       Filename:  count.c
//
//    Description:  A program where parent process counts number of vowels
//                  in the given sentence and child process will count number
//                  of words in the same sentence.
//
//        Version:  1.0
//        Created:  02/11/2016 03:11:45 AM
//       Compiler:  g++
//         Author:  Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016, Jason Meng
// ------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int is_vowel(char ch) {
    char vowels[] = "aeiouyAEIOUY";
    for (int i = 0; i < strlen(vowels); ++i)
        if (ch == vowels[i])
            return 1;
    return 0;
}


int is_splitter(char ch) {
    char splitters[] = " ,;.!-_~@#$%^&*()+={}[]|\n\t";
    for (int i = 0; splitters[i] != '\0'; ++i)
        if (ch == splitters[i])
            return 1;
    return 0;
}


int main(int argc, char *argv[]) {
    char help[] = "Usages: ./count <sentence>\n<sentence> cannot be longer than 100 characters.\n";
    char buffer[100];
    int status, pid, size, count = 0, index = 0;

    if (argc != 2) {
        printf("Wrong command.\n%s\n", help);
        return -1;
    }

    // copy the sentence from arg
    strcpy(buffer, argv[1]);

    pid = fork();
    if (pid == 0) {
        // count words
        while (buffer[index] != '\0') {
            if (is_splitter(buffer[index]) != 1) {
                count++;
                // increase index until meets the next splitter
                while (is_splitter(buffer[index]) != 1 && buffer[index] != '\0')
                    index++;
            }

            if (buffer[index] != '\0')
                index++;
        }

        printf("Words count: %d\n", count);
    } else {
        wait(&status);
        if (pid == -1) {
            printf("Failed to fork\n");
            return -1;
        }

        size = strlen(buffer);
        for (int i = 0; i < size; ++i)
            if (is_vowel(buffer[i]) == 1) count++;

        printf("Vowels count: %d\n", count);
    }

    return 0;
}
