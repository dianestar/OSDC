#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void producer();
void consumer();

int buffer[100];
int rear=-1;
int front=-1;

sem_t empty_slot;
sem_t data_items;

void producer() {
	int i;
	for (i=0;i<1000;i++) {
		printf("producer data = %d\n", i);
		
		sem_wait(&empty_slot);

		rear++;
		rear%=100;
		buffer[rear]=i;

		sem_post(&data_items);
	}
}

void consumer() {
	int i, data;
	for (i=0;i<1000;i++) {
		sem_wait(&data_items);

		front++;
		front%=100;
		data=buffer[front];

		sem_post(&empty_slot);

		printf("\t\tconsumer data = %d\n", data);
	}
}

int main(void) {
	int i;
	
	sem_init(&empty_slot, 0, 100);
	sem_init(&data_items, 0, 0);

	pthread_t thread_producer, thread_consumer;
	pthread_create(&thread_producer, NULL, (void *)&producer, NULL);
	pthread_create(&thread_producer, NULL, (void *)&consumer, NULL);

	pthread_join(thread_producer, NULL);
	pthread_join(thread_consumer, NULL);

	sem_destroy(&empty_slot);
	sem_destroy(&data_items);
}
