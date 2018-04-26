//Deadlock case 1
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

		

void main(){

	int fd1,fd2;
	fd1 = open(DEVICE, O_RDWR);

	printf("Trying to open device the first time\n");
	if(fd1 == -1) {
		printf("File %s either does not exist or has been locked by another "
				"process\n", DEVICE);
		exit(-1);
	}

	printf("Trying to open device the second time\n");
	fd2 = open(DEVICE, O_RDWR);
	if(fd2 == -1) {
		printf("File %s either does not exist or has been locked by another "
				"process\n", DEVICE);
		exit(-1);
	}

	printf("closing device once\n");
	close(fd1);
	printf("closing device once again\n");
	close(fd1);

}