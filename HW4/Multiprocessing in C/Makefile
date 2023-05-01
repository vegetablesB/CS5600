all: main

main: polybius.o main.o
	gcc polybius.o main.o -o main

polybius.o: polybius.c polybius.h
	gcc -c polybius.c

main.o: main.c polybius.h
	gcc -c main.c

run: main
	./main text.txt

clean:
	rm -f *.o main *output*

tar:
	tar -cvf CaoN.CS5600.S23.tar main.c polybius.c polybius.h text.txt blank.txt Makefile README