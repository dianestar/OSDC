CC = gcc

all : writer_Aprocess reader_Bprocess

writer_Aprocess : writer_Aprocess.o
	gcc -o writer_Aprocess writer_Aprocess.o

writer_Aprocess.o : writer_Aprocess.c
	gcc -c -o writer_Aprocess.o writer_Aprocess.c

reader_Bprocess : reader_Bprocess.o
	gcc -o reader_Bprocess reader_Bprocess.o

reader_Bprocess.o : reader_Bprocess.c
	gcc -c -o reader_Bprocess.o reader_Bprocess.c

clean :
	rm *.o writer_Aprocess
	rm *.o reader_Bprocess
