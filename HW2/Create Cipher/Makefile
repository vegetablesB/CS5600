all: cipher

cipher: polybius.o cipher.o
	gcc polybius.o cipher.o -o cipher

polybius.o: polybius.c polybius.h
	gcc -c polybius.c

cipher.o: cipher.c polybius.h
	gcc -c cipher.c

clean:
	rm -f *.o cipher