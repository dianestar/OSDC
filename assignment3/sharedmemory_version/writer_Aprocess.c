#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(void) {
	int shmid;
	//int i;
	void *shmaddr;
	char msg[100];

	//make a shared memory
	shmid=shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if (shmid<0) {
		perror("shmget");
		return 0;
	}

	//attach the shared memory
	shmaddr=shmat(shmid, (void *)0, 0);
	if (shmaddr==(char *)-1) {
		perror("attach failed\n");
		return 0;
	}

	printf("Hello, this is B process. I'll give the data to B.\n");
	while (1) { //infinite loop
		fgets(msg, sizeof(msg), stdin); //get user's input by delimiter '\n'
		msg[strlen(msg)-1]='\0'; //delete '\n' included in msg
		strcpy((char*)shmaddr, msg); //save the data in shared memory
	}

	return 0;
}
