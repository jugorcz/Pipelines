CC=gcc
CFLAGS= -Wall
%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

all: main tmp clean

main: main.o
	$(CC) -o $@ $< $(CFLAGS)
	
tmo: tmp.o
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm *.o
	
cleanAll:
	rm main
	rm tmp
