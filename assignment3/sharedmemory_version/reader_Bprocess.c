#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {
	int shmid;
	void *shmaddr;
	int ret;
	struct datacheck {
		int check;
	} *shdata;

	//get shared memory id
	shmid=shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if (shmid==-1) {
		perror("shared memory access is failed\n");
		return 0;
	}

	//attach the shared memory
	shmaddr=shmat(shmid, (void *)0, 0);
	if (shmaddr==(char *)-1) {
		perror("attach failed\n");
		return 0;
	}
	
	//if shdata->check==0, there's no newly arrived data
	shdata=(struct datacheck*)shmaddr;
	shdata->check=0;

	printf("Hello, this is B process. give me the data.\n");
	while (1) {	//infinite loop
		if (shdata->check) { //if new data has arrived in shared memory
			printf("%s\n", (char *)shmaddr); //print the received data
			shdata->check=0;
		}
	}

	//detach the shared memory
	ret=shmdt(shmaddr);
	if (ret==-1) {
		perror("detach failed\n");
		return 0;
	}

	//remove the shared memory
	ret=shmctl(shmid, IPC_RMID, 0);
	if (ret==-1) {
		perror("remove failed\n");
		return 0;
	}

	return 0;
}
