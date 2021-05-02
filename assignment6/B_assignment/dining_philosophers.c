#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define EATING_COUNT 3 //suppose each philosopher eats three times
sem_t chopstick[6];

void* oddPhilosopher(void *n) {
	//philosopher ID
	int id=*((int *)n);
	//left chopstick
	int left=(id+5)%6;
	//right chopstick
	int right=id;

	int i;
	for (i=0;i<EATING_COUNT;i++) {
		//picking up left chopstick first
		sem_wait(&chopstick[left]);
		printf("philosopher%d picks up chopstick%d\n", id, left);

		//picking up right chopstick next
		sem_wait(&chopstick[right]);
		printf("philosopher%d picks up chopstick%d\n", id, right);

		//eating
		printf("philosopher%d EATS\n", id);
		usleep(100000);

		//putting down left chopstick
		sem_post(&chopstick[left]);
		printf("philosopher%d puts down chopstick%d\n", id, left);

		//putting down right chopstick
		sem_post(&chopstick[right]);
		printf("philosopher%d puts down chopstick%d\n", id, right);

		//thinking
		printf("philosopher%d THINKS\n", id);
		usleep(300000);
	}
}

void* evenPhilosopher(void *n) {
	//philosopher ID
	int id=*((int *)n);
	//left chopstick
	int left=(id+5)%6;
	//right chopstick
	int right=id;

	int i;
	for (i=0;i<EATING_COUNT;i++) {
		//picking up right chopstick first
		sem_wait(&chopstick[right]);
		printf("philosopher%d picks up chopstick%d\n", id, right);

		//picking up left chopstick next
		sem_wait(&chopstick[left]);
		printf("philosopher%d picks up chopstick%d\n", id, left);

		//eating
		printf("philosopher%d EATS\n", id);
		usleep(100000);

		//putting down right chopstick
		sem_post(&chopstick[right]);
		printf("philosopher%d puts down chopstick%d\n", id, right);

		//putting down left chopstick
		sem_post(&chopstick[left]);
		printf("philosopher%d puts down chopstick%d\n", id, left);

		//thinking
        printf("philosopher%d THINKS\n", id);
        usleep(300000);
	}
}

int main(void) {
	pthread_t philosopher[6];
	int philosopherID[6]={0,1,2,3,4,5};
	int i;

	for (i=0;i<6;i++) { //initialize chopstick semaphores
		sem_init(&chopstick[i],0,1);
	}
	for (i=0;i<6;i++) { //create philosopher threads
		if (i%2==1) { //asymmetric solution for odd philosopher
			pthread_create(&philosopher[i], NULL, oddPhilosopher, (void *)&philosopherID[i]);
		}
		else { //asymmetric solution of even philosopher
			pthread_create(&philosopher[i], NULL, evenPhilosopher, (void *)&philosopherID[i]);
		}
	}
	for (i=0;i<6;i++) { //wait for threads to be ended
		pthread_join(philosopher[i], NULL);
	}
	for (i=0;i<6;i++) { //destroy semaphores
		sem_destroy(&chopstick[i]);
	}
}
