------------------------------------
PROGRAM: newdiff
Author : Jason Qiao Meng
ID     : 10652564
Email  : jason.meng@sju.edu
-----------------------------------

Description
-----------
This program compares 2 text files and gives the count of the differences.
The program does the comparison in 3 modes:
    Comparison on characters
    Comparison on words
    Comparison on lines


Files and Directories
---------------------
file1.txt                       // demo text file to feed newdiff
file2.txt                       // demo text file to feed newdiff
newdiff.cpp                     // main program to do the diff
README.txt                      // this file


To Complile
-----------
1. Unizp the package and save the directories under a certain directory, i.e.: ~\source.
2. Launch a terminal environment, change current directory to ~\source.
3. run gcc with the options:
    gcc -O3 -std=c++11 -Wstrict-overflow -Wno-missing-field-initializers -Wno-missing-field-initializers -Wall -o newdiff newdiff.cpp


To Run
------
The command is as below:
    ./newdiff [MODE] <file1> <file2>
E.g.: ./newdiff -c file1.txt file2.txt

[EOF]

