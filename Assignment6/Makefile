obj-m += assignment6.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o deadlock1 deadlock1.c -lpthread
	gcc -o deadlock2 deadlock2.c -lpthread
	gcc -o deadlock3 deadlock3.c -lpthread
	gcc -o deadlock4 deadlock4.c -lpthread

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm deadlock1
	rm deadlock2
	rm deadlock3
	rm deadlock4

deadlock1: 
	gcc -o deadlock1 deadlock1.c -lpthread

deadlock1: 
	gcc -o deadlock2 deadlock2.c -lpthread

deadlock1: 
	gcc -o deadlock3 deadlock3.c -lpthread

deadlock1: 
	gcc -o deadlock4 deadlock4.c -lpthread
