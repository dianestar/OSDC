#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define COUNTING_NUMBER 2000000

struct smStruct {
	int processidassign;
	int turn;
	int flag[2];
	int critical_section_variable;
};

int main(void) {
	int shmid;
	void *shmaddr;
	int ret;

	struct smStruct *smstruct;

	pid_t pid1, pid2;
	int status1, status2;

	//make a shared memory
	shmid=shmget((key_t)1234, 1024, IPC_CREAT|0666);
	if (shmid<0) {
		perror("shmget failed\n");
		return 0;
	}

	//attach the shared memory
	shmaddr=shmat(shmid, (void *)0, 0);
	if (shmaddr==(char *)-1) {
		perror("attach failed\n");
		return 0;
	}

	//link smstruct to the shared memory and initialize smstruct
	smstruct=shmaddr;
	smstruct->turn=0;
	smstruct->processidassign=0;
	smstruct->flag[0]=0;
	smstruct->flag[1]=0;
	smstruct->critical_section_variable=0;

	if ((pid1=fork())==0) { //create child process #1
		execl("./child", "./child", NULL); //exec child program
	}
	else { 
		if ((pid2=fork())==0) { //create child process #2
			execl("./child", "./child", NULL); //exec child program
		}
		else {
			//wait until child processes exit
			while (!waitpid(pid1, &status1, 0) || !waitpid(pid2, &status2, 0));
			//print actual count and expected count
			printf("Actual Count: %d | Expected Count: %d\n", smstruct->critical_section_variable, COUNTING_NUMBER*2);
		}
	}

	//detach the shared memory
	shmdt(shmaddr);
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
