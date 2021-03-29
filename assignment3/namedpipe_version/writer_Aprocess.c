#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MSG_SIZE 80
#define PIPENAME "./named_pipe_file"

int main(void) {
	char msg[MSG_SIZE];
	int fd;
	int ret, i;

	fd=open(PIPENAME, O_WRONLY); //open the named pipe with write only option
	if (fd<0) { 
		printf("Open failed\n");
		return 0;
	}

	printf("Hello, this is A process. I'll give the data to B.\n");
	while (1) { //infinite loop
		fgets(msg, sizeof(msg), stdin); //get user's input by delimiter '\n'
		msg[strlen(msg)-1]='\0'; //delete '\n' included in msg
		ret=write(fd, msg, sizeof(msg)); //write(produce) data on the pipe
		if (ret<0) {
			printf("Write failed\n");
			return 0;
		}
	}

	return 0;
}
