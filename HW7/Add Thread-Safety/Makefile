CC = gcc
CFLAGS = -Wall -Werror -pthread
TARGETS = pm_heap_test pm_heap_test_thread

all: $(TARGETS)

pm_heap_test: pm_heap_test.o pm_heap.o
	$(CC) $(CFLAGS) -o $@ $^

pm_heap_test_thread: pm_heap_test_thread.o pm_heap.o
	$(CC) $(CFLAGS) -o $@ $^

pm_heap_test.o: pm_heap_test.c pm_heap.h
	$(CC) $(CFLAGS) -c $<

pm_heap_test_thread.o: pm_heap_test_thread.c pm_heap.h
	$(CC) $(CFLAGS) -c $<

pm_heap.o: pm_heap.c pm_heap.h
	$(CC) $(CFLAGS) -c $<

run: pm_heap_test_thread
	./pm_heap_test_thread

run1: pm_heap_test
	./pm_heap_test

clean:
	rm -f $(TARGETS) *.o

tar:
	tar -cvf HW7.tar pm_heap.c pm_heap.h pm_heap_test.c pm_heap_test_thread.c Makefile README
