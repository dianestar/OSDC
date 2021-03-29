#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPENAME "./named_pipe_file"

int main() {
	int ret;
	char msg[512];
	int fd;
	pid_t pid;

	//delete if named pipe exists
	ret=access(PIPENAME, F_OK);
	if (ret==0) {
		unlink(PIPENAME); 
	}

	//create a named pipe
	ret=mkfifo(PIPENAME, 0666);
	if (ret<0) { 
		printf("Creation of named pipe failed\n");
		return 0;
	}

	fd=open(PIPENAME, O_RDWR); //open the named pipe with read/write option
	if (fd<0) {
		printf("Opening of named pipe failed\n");
		return 0;
	}

	printf("Hello, this is B process. give me the data.\n");
	while (1) { //infinite loop
		ret=read(fd, msg, sizeof(msg)); //read data from the pipe
		if (ret<0) {
			printf("Read failed\n");
			return 0;
		}
		printf("%s\n", msg); //print the data
	}
	return 0;
}
