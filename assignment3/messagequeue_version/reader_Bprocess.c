#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct msgbuf {
	long msgtype;
	char mtext[20];
};

int main(int argc, char **argv) {
	key_t key_id;
	struct msgbuf rsvbuf;
	int msgtype=3;

	key_id=msgget((key_t)1234, IPC_CREAT|0666); //return id of message queue

	if (key_id==-1) {
		perror("msgget error : ");
		return 0;
	}

	printf("Hello, this is B process. give me the data.\n");
	while (1) { //infinite loop
		//receive data from message queue and return error if failed
		if (msgrcv(key_id, (void *)&rsvbuf, sizeof(struct msgbuf), msgtype, 0)==-1) {
			perror("msgrcv error :");
		}
		else {
			printf("%s\n", rsvbuf.mtext); //print the received data
		}
	}
	
	return 0;
}
