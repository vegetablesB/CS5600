CC=gcc
CFLAGS=-c -Wall

all: qtest

qtest: queue.o queuetest.o
	$(CC) queue.o queuetest.o -o qtest

queue.o: queue.c
	$(CC) $(CFLAGS) queue.c

queuetest.o: queuetest.c
	$(CC) $(CFLAGS) queuetest.c

clean:
	rm -f *.o qtest
