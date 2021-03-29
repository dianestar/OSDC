CC = gcc

all : child1_exec child2_exec assignment2

child1_exec : child1_exec.o
	gcc -o child1_exec child1_exec.o

child1_exec.o : child1_exec.c
	gcc -c -o child1_exec.o child1_exec.c

child2_exec : child2_exec.o
	gcc -o child2_exec child2_exec.o

child2_exec.o : child2_exec.c
	gcc -c -o child2_exec.o child2_exec.c

assignment2 : assignment2.o
	gcc -o assignment2 assignment2.o

assignment2.o : assignment2.c
	gcc -c -o assignment2.o assignment2.c

clean :
	rm *.o child1_exec
	rm *.o child2_exec
	rm *.o assignment2
