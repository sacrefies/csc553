------------------------------------
PROGRAM: Range Summation and Prime Searching
Author : Jason Qiao Meng
ID     : 10652564
Email  : jason.meng@sju.edu
-----------------------------------

Description
-----------
This package includes 3 programs which are multithreading exercises:
  - A C program which implements a multithreading mechanism to search the primes;
  - A C program which implements a multithreading mechanism to do range summation.
  - A java program which is the equivalent of the range summation program in C.


Files and Directories
---------------------
hw3
 |- Accumulator.java
 |- primes.c
 |- sum.c
 |- Summation.java
 |- README.txt                                             // This file


To Compile
-----------
1. Unizp the package and save the directories under a certain directory, i.e.: ~\source.
2. Launch a command line environment, change current directory to ~\source.
3. Presume JDK bin directory exists in PATH and JAVA_HOME are correctly set,
   Run the below command:
     javac -cp . *.java

   Note: This program is developed on top of JDK1.7.
         Compilation error would happen if a javac with a lower version is applied.
4. Run the below commands for the C programs:
    $ gcc -Wall -std=c99 -O3 -pthread -o primes primes.c
    $ gcc -Wall -std=c99 -O3 -pthread -o sum sum.c

To Run
------
The command is as below:
  $ java -Xmx512 -cp . Summation <lower_bound> <upper_bound>
  $ ./sum <lower_bound> <upper_bound>
  $ ./primes <N>


[EOF]
