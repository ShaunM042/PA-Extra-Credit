CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
second: second.o
	$(CC) $(CFLAGS) -o second second.o
second.o: second.c second.h
	$(CC) $(CFLAGS) -c second.c
clean:
	rm -f *.o second