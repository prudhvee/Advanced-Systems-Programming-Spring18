This submission contains the following files:

1) mapper.c 
	- contains the implementation of mapper logic
	- reads tuples from input.txt
	- reads rules from rules.txt
	- outputs to stdout 

2) reducer.c 
	- contains the implementaion of reducer logic for tuples
	- reads from stdin
	- outputs to stdout whenever no more tuples of the same user id are coming in 

3) combiner.c
	- forks and creates 2 children, child1: mapper , child2: reducer & connects them using a pipe

4) input.txt 
	- contains input tuples either on the same line or on different lines

5) rules.txt
	- contains the rules eg: P=50
	- each rule is on a new line and is in the form char=val

6) Makefile
	- use make or make all to compile all files
	- use make mapper or similar to compile specific files 
	- use make run to compile and run in one shot

7) Readme.txt 
	- this file

use ./combiner to run explicitly.
