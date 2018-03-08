CC=gcc
CFLAGS= -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Problem2: Problem0.o
	gcc -o Problem0 Problem0.o -lm