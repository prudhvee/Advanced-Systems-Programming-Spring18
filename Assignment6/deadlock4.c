#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>

#define DEVICE "/dev/a5"

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IOWR(CDRV_IOC_MAGIC, 1, int)
#define E2_IOCMODE2 _IOWR(CDRV_IOC_MAGIC, 2, int)


pthread_mutex_t lock;
pthread_cond_t cond;
int sharedvar=0;

void *func1()
{
	int fd;

	sleep(1);
	fd = open(DEVICE, O_RDWR);
	if(fd == -1)
	{
		printf("File %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}
	

	while(sharedvar<2);
	ioctl(fd,E2_IOCMODE2,0);
	
	pthread_mutex_lock(&lock);
	pthread_cond_wait(&cond,&lock);
	pthread_mutex_unlock(&lock);
	close(fd);
	pthread_exit(NULL);
}

void *func2()
{
	int fd;
	fd = open(DEVICE, O_RDWR);
	if(fd == -1)
	{
		printf("File %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}
	ioctl(fd,E2_IOCMODE2,0);

	while(sharedvar<2);
	ioctl(fd,E2_IOCMODE1,0);
	pthread_cond_signal(&cond);
	close(fd);
	pthread_exit(NULL);
}


int main ()
{
   	pthread_t tid1,tid2;
   	int rc1,rc2;
   	long t;
      
    rc1 = pthread_create(&tid1, NULL, func1, NULL);
	rc2 = pthread_create(&tid2, NULL, func2, NULL);
   	pthread_exit(NULL);
}