#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COUNTING_NUMBER 2000000

using namespace std;

int critical_section_variable=0;

//declare shared variables as atomic_int
atomic_int turn(0);
atomic_int flag0(0), flag1(0);

void lock(int self) {
	if (self==0) { //for thread 0
		flag0.store(1); //signal that thread0 wants to enter critical section
		turn.store(1); //let thread1 to use critical section first
		while (flag1.load() && turn.load()==1); //if thread1 is using, wait!
	/* to decrease the number of mfence, maybe we can use the code below in this block, instead!
		flag0.store(1);
		turn.store(1, memory_order_release);
		while (flag1.load(memory_order_acquire) && turn.load(memory_order_acquire));
	*/
	}
	else { //for thread 1
		flag1.store(1); //signal that thread1 wants to enter critical section
		turn.store(0); //let thread0 to use critical section first
		while (flag0.load() && turn.load()==0); //if thread0 is using, wait!
	/* to decrease the number of mfence, maybe we can use the code below in this block, instead!
		flag1.store(1);
		turn.store(0, memory_order_release);
		while (flag0.load(memory_order_acquire) && turn.load(memory_order_acquire));
	*/
	}
}

void unlock(int self) {
	if (self==0) { //for thread 0
		flag0.store(0); //thread0 escapes critical section and returns lock
	/* to decrease the number of mfence, maybe we can use the code below in this block, instead!
		flag0.store(0, memory_order_release);
	*/
	}
	else { //for thread 1
		flag1.store(0); //thread1 escapes critical section and returns lock
	/* to decrease the number of mfence, maybe we can use the code blow in this block, instead!
		flag1.store(0, memory_order_release);
	*/
	}
}

void* func(void *s) {
	int* thread_num=(int *)s;
	int i;
	for (i=0;i<COUNTING_NUMBER;i++) {
		lock(*thread_num);
		critical_section_variable++; //critical section
		unlock(*thread_num);
	}
}

int main(void) {
	pthread_t p1, p2;

	int parameter[2]={0,1};

	pthread_create(&p1, NULL, func, (void*)&parameter[0]);
	pthread_create(&p2, NULL, func, (void*)&parameter[1]);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	printf("Actual Count: %d | Expected Count: %d\n", critical_section_variable, COUNTING_NUMBER*2);

	return 0;
}
