CC=gcc
CFLAGS= -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Problem1: Problem1.o
	gcc -o Problem1 Problem1.o -lm
