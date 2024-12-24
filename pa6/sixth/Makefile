CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
sixth: sixth.o
	$(CC) $(CFLAGS) -o sixth sixth.o
sixth.o: sixth.c sixth.h
	$(CC) $(CFLAGS) -c sixth.c
clean:
	rm -f *.o sixth