CC = gcc

BitApp: iss.o
	$(CC) -o simpleISS iss.c
clean: 
	rm simpleISS iss.o
