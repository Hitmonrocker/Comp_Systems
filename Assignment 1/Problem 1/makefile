CC=gcc
CFLAGS= -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Problem2: Problem2.o
	gcc -o Problem2 Problem2.o -lm