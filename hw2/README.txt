------------------------------------
PROGRAM: count, pipe, sort
Author : Jason Qiao Meng
ID     : 10652564
Email  : jason.meng@sju.edu
-----------------------------------

Description
-----------
This project contains source files of 3 programs which are CSC553 homework 2.
  pipe.c    A Linux pipe demonstration
  count.c   A Linux process control demonstration
  sort.c    A Linux process control demonstration
The c sources are developed and tested on Linux environment. They are NOT
compatible with a WIN32/64 environment.

Files and Directories
---------------------
pipe.c
count.c
sort.c
pid.c                           // This source file is to demo pids after fork()
README.txt                      // this file


To Complile
-----------
1. Unizp the package and save the directories under a certain directory, i.e.: ~\source.
2. Launch a terminal environment, change current directory to ~\source.
3. run gcc with the options:
    gcc -O3 -std=c11 -Wall -o <out_file> <source_c_file>


To Run
------
The commands are illustrated as below:
    ./count "  hello world  , this is MINE"
    ./pipe
    ./sort

[EOF]
