#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Lock variable declaration
pthread_mutex_t mutex_lock;

int result=0;

void *ThreadFunc(void *n) {
	int i;
	int *number=(int *)n;
	for (i=0;i<3;i++) {
		//Locking
		pthread_mutex_lock(&mutex_lock);
		printf("%dth process :%d\n", *number, result++);
		//Unlocking
		pthread_mutex_unlock(&mutex_lock);
	}
	return 0;
}

int main(void) {
	pthread_t threadID[5];

	//Init the lock
	pthread_mutex_init(&mutex_lock, NULL);

	int argument[5]={1,2,3,4,5};
	int i;

	for (i=0;i<5;i++) {
		pthread_create(&(threadID[i]), NULL, ThreadFunc, (void *)&argument[i]);
	}

	printf("Main Thread is waiting for Child Thread!\n");

	for (i=0;i<5;i++) {
		pthread_join(threadID[i], NULL);
	}

	return 0;
}
