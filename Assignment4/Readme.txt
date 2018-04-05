This is a submission for Assignment4. This does not make use of the simplification
A new consumer process is forked whenever a new unique tuple id is found. 

- compile using "make"
- run the program with the command "./combiner 10 < input.txt" where 10 is the size of the buffer

- the submission contains the following files.
1) combiner.c 
	- contains the implementation of mapper-reducer 
	- mapper processor forks n reducer processes 
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

