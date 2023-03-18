CC = gcc
CFLAGS = -Wall -Wextra -g

OBJS = pm_heap.o pm_heap_test.o

all: test

test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lpthread

pm_heap.o: pm_heap.c pm_heap.h
	$(CC) $(CFLAGS) -c -o $@ pm_heap.c

test.o: pm_heap_test.c pm_heap.h
	$(CC) $(CFLAGS) -c -o $@ pm_heap_test.c

.PHONY: clean
clean:
	rm -f *.o pm_heap_test
