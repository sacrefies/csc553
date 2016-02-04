// ------------------------------------------------------------------------------------
//            $Id$
//          $Date$
//       Filename:  newdiff.cpp
//
//    Description:  This cpp file contains functions with different approaches to compare
//                  content in 2 text files.
//
//        Version:  1.0
//        Created:  02/03/2016 07:31:49 PM
//       Compiler:  g++
//         Author:  Jason Meng (jm), jm652564@sju.edu
//
// Copyright (c) 2016, Jason Meng
// ------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

/**
 * Compare char by char in the two files and returns
 * the count number of the different characters.
 */
int compareChars(FILE *src, FILE *dst) {
    int cursorsource, cursordest;
    int diffcount = 0;

    cursorsource = fgetc(src);
    cursordest = fgetc(dst);
    while (cursorsource != EOF && cursordest != EOF) {
        if (cursorsource != cursordest) {
            diffcount++;
        }
        cursorsource = fgetc(src);
        cursordest = fgetc(dst);
    }

    while (cursorsource != EOF) {
        diffcount++;
        cursorsource = fgetc(src);
    }

    while (cursordest != EOF) {
        diffcount++;
        cursordest = fgetc(dst);
    }

   return diffcount;
}

int compareWords(FILE *src, FILE *dst) {
    int diffcount;
    char srcbuff[50];
    char destbuff[50];
    char srcflag, destflag;

    diffcount = 0;
    srcflag = fscanf(src, "%s", srcbuff);
    destflag = fscanf(dst, "%s", destbuff);
    while (srcflag != EOF && destflag != EOF) {
        if (strcmp(srcbuff, destbuff) != 0) {
            diffcount++;
        }
        srcflag = fscanf(src, "%s", srcbuff);
        destflag = fscanf(dst, "%s", destbuff);
    }
    while (srcflag != EOF) {
        diffcount++;
        srcflag = fscanf(src, "%s", srcbuff);
    }

    while ( destflag != EOF) {
        diffcount++;
        destflag = fscanf(dst, "%s", destbuff);
    }

    return diffcount;
}

int compareLines(FILE *src, FILE *dst) {
    char *srcline = NULL;
    char *destline = NULL;
    size_t srclen = 0;
    size_t destlen = 0;
    int srcflag, destflag, diffcount;

    srcflag = getline(&srcline, &srclen, src);
    destflag = getline(&destline, &destlen, dst);
    diffcount = 0;

    while (srcflag != -1 && destflag != -1) {
        if (strcmp(srcline, destline) != 0) {
            diffcount ++;
        }
        srcflag = getline(&srcline, &srclen, src);
        destflag = getline(&destline, &destlen, dst);
    }

    while (srcflag != -1) {
        diffcount ++;
        srcflag = getline(&srcline, &srclen, src);
    }

    free(srcline);

    while (destflag != -1) {
        diffcount ++;
        destflag = getline(&destline, &destlen, dst);
    }

    free(destline);

    return diffcount;
}

/**
 * Apply diff method to the files
 */
int diff(int (*func)(FILE*, FILE*), const char *sourcefile, const char *destfile) {
    int count = 0;

    FILE *src = fopen(sourcefile, "r");
    if (src == NULL) {
        printf("Cannot open file %s\n", sourcefile);
        return -1;
    }

    FILE *dst = fopen(destfile, "r");
    if (dst == NULL) {
        printf("Cannot open file %s\n", destfile);
        return -1;
    }

    count = (*func)(src, dst);

    fclose(src);
    fclose(dst);

    if (count > 0 ) {
        printf("Files conatins %d differences.", count);
    }

    return 0;
}


/**
 * The main execution entrance point of this application.
 *
 * @param argc The count of the command line arguments.
 * @param argv The command line arguments where argv[0]
 *            is this program's name.
 * @return Returns 0 if no error happens.
 */
int main(const int argc, const char *argv[]) {
    if (argc < 4) {
        printf("Wrong argument input\n");
        return -1;
    }

    if (strcmp(argv[1], "-w") != 0 &&
        strcmp(argv[1], "-c") != 0 &&
        strcmp(argv[1], "-l") != 0) {
        printf("Comparison approach is not given. -w/-c/-l");
        return -1;
    }

    if (strcmp(argv[1], "-w") == 0) {
       return diff(compareWords, argv[2], argv[3]);
    }

    if (strcmp(argv[1], "-c") == 0) {
        return diff(compareChars, argv[2], argv[3]);
    }

    if (strcmp(argv[1], "-l") == 0) {
        return diff(compareLines, argv[2], argv[3]);
    }

     return 0;
}

