#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //header for using threads

#define ARGUMENT_NUMBER 20

long long result=0;
void *ThreadFunc(void *n) {
	long long temp=0; //a variable to temporarily save the result of each threads
	long long i;
	long long number=*((long long*)n);
	printf("number=%lld\n", number);
	for (i=0;i<25000000;i++) {
		temp+=number;
	}
	result+=temp; //maintain consistency without using lock by using 'temp'
	return 0;
}

int main(void) {
	pthread_t threadID[20]; //declare pthread_t structures by ARGUMENT_NUMBER
	long long argument[ARGUMENT_NUMBER];
	long long i;
	for (i=0;i<ARGUMENT_NUMBER;i++) {
		argument[i]=i;
	}
	for (i=0;i<ARGUMENT_NUMBER;i++) {
		pthread_create(&(threadID[i]), NULL, ThreadFunc, (void *)&argument[i]); //create threads
	}
	printf("Main Thread is waiting for Child Thread!\n");
	for (i=0;i<ARGUMENT_NUMBER;i++) {
		pthread_join(threadID[i], NULL); //wait for all threads to be terminated
	}
	printf("result=%lld\n", result); //print 'result'
	return 0;
}
