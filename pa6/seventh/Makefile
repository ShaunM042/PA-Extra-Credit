CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
seventh: seventh.o
	$(CC) $(CFLAGS) -o seventh seventh.o
seventh.o: seventh.c seventh.h
	$(CC) $(CFLAGS) -c seventh.c
clean:
	rm -f *.o seventh
