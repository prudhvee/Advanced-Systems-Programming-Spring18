all: 
	gcc -g -Wall -o mapper mapper.c
	gcc -g -Wall -o reducer reducer.c
	gcc -g -Wall -o combiner combiner.c

mapper:
	gcc -g -Wall -o mapper mapper.c
reducer:
	gcc -g -Wall -o reducer reducer.c
combiner:
	gcc -g -Wall -o combiner combiner.c

clean: 
	$(RM) mapper
	$(RM) reducer
	$(RM) combiner

run: 
	gcc -g -Wall -o mapper mapper.c
	gcc -g -Wall -o reducer reducer.c
	gcc -g -Wall -o combiner combiner.c
	./combiner
