This is a submission for Assignment2 and not Assignment2 Simplified. 

- compile using "make"
- run the program with the command "./combiner 10 <input.txt" where 10 is the size of the buffer
- new reducer threads are spawned whenever the mapper encounters a new thread 

- the submission contains the following files.
1) combiner.c 
	- contains the implementation of mapper-reducer logic
	- spans 1 mapper thread and n reducer threads
	- reads rules from rules.txt
	- outputs to stdout 

2) input.txt 
	- contains input tuples either on the same line or on different lines

3) rules.txt
	- contains the rules eg: P=50
	- each rule is on a new line and is in the form char=val

4) Makefile
	- use make or make all to compile all files
	- use male clean to delete binary

5) Readme.txt 
	- this file

use ./combiner to run explicitly.
