CC=gcc
CFLAGS= -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mathTree: mathTree.o
	gcc -o mathTree mathTree.o -lm
