CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
eighth: eighth.o
	$(CC) $(CFLAGS) -o eighth eighth.o
eighth.o: eighth.c eighth.h
	$(CC) $(CFLAGS) -c eighth.c
clean:
	rm -f *.o eighth