CC=gcc
CFLAGS=-c -Wall

all: CS5600.LearnC

CS5600.LearnC: CaoN.CS5600.S2.LearnC.o
	$(CC) CaoN.CS5600.S2.LearnC.o -o CS5600.LearnC

CaoN.CS5600.S2.LearnC.o: CaoN.CS5600.S2.LearnC.c
	$(CC) $(CFLAGS) CaoN.CS5600.S2.LearnC.c

clean:
	rm -f *.o CS5600.LearnC
