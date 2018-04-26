#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

#define DEVICE "/dev/a5"

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IOWR(CDRV_IOC_MAGIC, 1, int)
#define E2_IOCMODE2 _IOWR(CDRV_IOC_MAGIC, 2, int)

		
void *func2(){
	//thread 
	int fd,rc;

	fd = open(DEVICE, O_RDWR);
	if(fd == -1) {
		printf("File %s either does not exist or has been locked by another "
				"process\n", DEVICE);
		exit(-1);
	}
	sleep(1);
	printf("Thread1: Trying to set mode=1\n");
	rc = ioctl(fd,E2_IOCMODE1,0);
	close(fd);	
	printf("Thread1 finished\n");
	pthread_exit(NULL);
}

void *func1(){

	int rc,fd;
	fd = open(DEVICE, O_RDWR);
	if(fd == -1) {
		printf("File %s either does not exist or has been locked by another "
				"process\n", DEVICE);
		exit(-1);
	}

	sleep(1);

	printf("Thread2: Trying to set mode=1\n");
	//change to mode 2
	rc = ioctl(fd,E2_IOCMODE1,0);
	close(fd);	
	printf("Thread2 finished\n");
	pthread_exit(NULL);
	
}


void main(){

	int s,rc,fd;
	pthread_t tid;

	fd = open(DEVICE, O_RDWR);
	if(fd == -1) {
		printf("File %s either does not exist or has been locked by another "
				"process\n", DEVICE);
		exit(-1);
	}
	printf("setting mode=2\n");
	rc = ioctl(fd,E2_IOCMODE2,0);
	close(fd);

	s = pthread_create(&tid, NULL, func1, NULL);

	s = pthread_create(&tid, NULL, func2, NULL);
	pthread_exit(NULL);

}