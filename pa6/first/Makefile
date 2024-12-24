CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror

first: first.o
	$(CC) $(CFLAGS) -o first first.o

first.o: first.c first.h
	$(CC) $(CFLAGS) -c first.c

clean:
	rm -f *.o first