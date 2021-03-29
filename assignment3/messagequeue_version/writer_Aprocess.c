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

int main(void) {
	key_t key_id;
	struct msgbuf sndbuf;
	char msg[100];

	key_id=msgget((key_t)1234, IPC_CREAT|0666); //return id of message queue
	
	if (key_id==-1) {
		perror("msgget error : ");
		return 0;
	}
	
	printf("Hello, this is A process. I'll give the data to B.\n");
	while (1) { //infinite loop
		sndbuf.msgtype=3;
		fgets(msg, sizeof(msg), stdin); //get user's input by delimiter '\n'
		msg[strlen(msg)-1]='\0'; //delete '\n' included in msg
		strcpy(sndbuf.mtext, msg); //save data in message buffer struct

		//send data to message queue and return error if failed
		if (msgsnd(key_id, (void *)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT)==-1) {
			perror("msgsnd error : ");
		}
	}
	
	return 0;
}
