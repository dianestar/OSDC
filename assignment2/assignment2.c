#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t pid1, pid2;
	int status1, status2;

	printf("Waiting for Child Processes\n");

	if ((pid1=fork())==0) {
		execl("./child1_exec",NULL);
	}
	if ((pid2=fork())==0) {
		execl("./child2_exec",NULL);
	}

	waitpid(pid1,&status1,0);
	waitpid(pid2,&status2,0);
	printf("Child processes are exit (%d) (%d)\n",status1,status2);

	return 0;
}
