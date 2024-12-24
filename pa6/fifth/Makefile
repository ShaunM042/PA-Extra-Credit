CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
fifth: fifth.o
	$(CC) $(CFLAGS) -o fifth fifth.o
fifth.o: fifth.c fifth.h
	$(CC) $(CFLAGS) -c fifth.c
clean:
	rm -f *.o fifth