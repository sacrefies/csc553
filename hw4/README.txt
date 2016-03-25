------------------------------------
PROGRAM: Round Robin Scheduler Simulation
         and Unisex Bathroom Problem Solver
Author : Jason Qiao Meng
ID     : 10652564
Email  : jason.meng@sju.edu
-----------------------------------

Description
-----------
This package includes 2 programs which are multithreading exercises:
  - A C program which simulates round-robin process scheduler
  - A java program which solves the unisex Bathroom problem


Files and Directories
---------------------
hw4
 |- headers
      |- queue.h            // C header file for circular queue functions
      |- utils.h            // C header file for some helper functions
 |- circularqueue.c         // C circular queue implementation
 |- round_robin.c           // C R-R scheduler implementation
 |- utils.c                 // C helper functions
 |- UnisexBathroom.java     // Unisex Bathroom solution in JAVA
 |- README.txt              // This file


To Compile
-----------
1. Unizp the package and save the directories under a certain directory, i.e.: ~\source.
2. Launch a command line environment, change current directory to ~\source.
3. Presume JDK bin directory exists in PATH and JAVA_HOME are correctly set,
   Run the below command:
     javac -cp . csc553/hw4/*.java

   Note: This program is developed on top of JDK1.7.
         Compilation error would happen if a javac with a lower version is applied.
4. Run the below commands for the C programs:
    $ gcc -Wall -std=c99 -lpthread -o round_robin round_robin.c circularqueue.c utils.c


To Run
------
The command is as below:
  $ java -Xmx512 -cp . csc553.hw4.UnisexBathroom 10 10
  $ ./round_robin 3 7 12 15 20 5



[EOF]
