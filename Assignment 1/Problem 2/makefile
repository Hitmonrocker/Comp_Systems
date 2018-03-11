CC=gcc
CFLAGS= -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

problem2: problem2.o
	gcc -o problem2 problem2.o -lm
