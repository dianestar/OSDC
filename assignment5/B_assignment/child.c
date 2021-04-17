#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define COUNTING_NUMBER 2000000

struct smStruct { 
	int processidassign;
	int turn;
	int flag[2];
	int critical_section_variable;
};

void lock(struct smStruct *smstruct_, int Myorder_) {
	//use Myorder_ as flag number and represent Peterson's algorithm with variables in smstruct_
	smstruct_->flag[Myorder_]=1;
	smstruct_->turn=1-Myorder_;
	while (smstruct_->flag[1-Myorder_]==1 && smstruct_->turn==1-Myorder_);
}

void unlock(struct smStruct *smstruct_, int Myorder_) {
	//use Myorder_ as flag number and represent Peterson's algorithm with variables in smtruct_
	smstruct_->flag[Myorder_]=0;
}

int main(void) {
	int shmid;
	void *shmaddr;
	int ret;

	struct smStruct *smstruct;
	int Myorder;

	//declare and initialize a local variable to actually count the # of critical_section_variable++ action
	int localcount=0;

	//make a shared memory
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

	//link smstruct to the shared memory
	smstruct=shmaddr;
	//number each processes
	Myorder=smstruct->processidassign++;
	//print Myorder and pid of itself
	printf("Myorder = %d,process pid = %d\n", Myorder, getpid());

	for (int i=0;i<COUNTING_NUMBER;i++) {
		localcount++; //actually count the # of increment action
		lock(smstruct, Myorder);
		smstruct->critical_section_variable++; //increase critical_section_variable
		unlock(smstruct, Myorder);
	}

	//print finish message and localcount
	printf(" child%d finish! local count = %d\n", Myorder, localcount);

	//detach the shared memory
	shmdt(shmaddr);
	if (ret==-1) {
		perror("detach failed\n");
		return 0;
	}

	return 0;
}
