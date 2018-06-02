CC=gcc
CFLAGS=-std=gnu99 -Wall

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
all: master slave clean

master: master.o
	$(CC) -o $@ $< $(CFLAGS)

slave: slave.o
	$(CC) -o $@ $< $(CFLAGS) -lm

clean:
	rm -f *.o

cleanAll:
	rm -f *.o
	rm -f pipe
	rm -f master
	rm -f slave