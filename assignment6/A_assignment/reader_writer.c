#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define COUNTING_NUMBER 2000000

sem_t S, wrt;
int readcount;
int cur_writer=0; //Critical Section variable
int cur_count=0; //Critical Section variable

void* writer(void *n) {
	int *writerID=(int *)n;
	int i;
	for (i=0;i<COUNTING_NUMBER;i++) {
		usleep(100000);
		sem_wait(&wrt); //LOCK(wait)
		cur_writer=*writerID; //Critical Section
		cur_count=i; //Critical Section
		sem_post(&wrt); //UNLOCK(signal)
	}
}

void* reader() {
	int i;
	for (i=0;i<COUNTING_NUMBER;i++) {
		usleep(30000);
		sem_wait(&S); //LOCK(wait) to prevent race condition of 'readcount++'
		readcount++; //plus the number of readers in Critical Section
		if (readcount==1) {
			sem_wait(&wrt); //LOCK(wait) to prevent writer access
		}
		sem_post(&S); //UNLOCK(signal) for readcount++
		printf("The most recent writer id:\t[%d], count:\t[%d]\n", cur_writer, cur_count); //Critical Section
		sem_wait(&S); //LOCK(wait) to prevent race condition of 'readcount--'
		readcount--; //minus the number of readers in Critical Section
		if (readcount==0) {
			sem_post(&wrt); //UNLOCK(signal) for writer
		}
		sem_post(&S); //UNLOCK(signal) for readcount--
	}
}

int main(void) {
	//initialize semaphores
	sem_init(&S, 0, 1);
	sem_init(&wrt, 0, 1);
	readcount=0;

	pthread_t thread_reader[2];
	pthread_t thread_writer[5];
	int id[5]={1,2,3,4,5};
	int i;

	for (i=0;i<2;i++) { //create two readers
		pthread_create(&(thread_reader[i]), NULL, reader, NULL);
	}
	for (i=0;i<5;i++) { //create five writers
		pthread_create(&(thread_writer[i]), NULL, writer, (void *)&id[i]);
	}

	for (i=0;i<2;i++) { //wait for reader threads to be ended
		pthread_join(thread_reader[i], NULL);
	}
	for (i=0;i<5;i++) { //wait for writer threads to be ended
		pthread_join(thread_writer[i], NULL);
	}

	//destroy semaphores
	sem_destroy(&S);
	sem_destroy(&wrt);
}
