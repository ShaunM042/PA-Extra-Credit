CC = gcc
CFLAGS = -fsanitize=address,undefined -Og -g -std=c11 -Wall -Werror
fourth: fourth.o
	$(CC) $(CFLAGS) -o fourth fourth.o
fourth.o: fourth.c fourth.h
	$(CC) $(CFLAGS) -c fourth.c
clean:
	rm -f *.o fourth