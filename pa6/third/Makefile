CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
third: third.o
	$(CC) $(CFLAGS) -o third third.o
third.o: third.c third.h
	$(CC) $(CFLAGS) -c third.c
clean:
	rm -f *.o third