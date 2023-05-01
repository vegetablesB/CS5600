all: pm_heap_test

pm_heap_test: pm_heap_test.o pm_heap.o
	cc -o pm_heap_test pm_heap_test.o pm_heap.o

pm_heap_test.o: pm_heap_test.c pm_heap.h
	cc -c pm_heap_test.c

pm_heap.o: pm_heap.c pm_heap.h
	cc -c pm_heap.c

run: pm_heap_test
	./pm_heap_test

clean:
	rm -f pm_heap_test pm_heap_test.o pm_heap.o

tar:
	tar -cvf HW6.tar pm_heap.c pm_heap.h pm_heap_test.c Makefile README