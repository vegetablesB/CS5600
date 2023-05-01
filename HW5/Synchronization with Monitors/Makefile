all: Main.class BoundedBuffer.class

Main.class: Main.java
	javac Main.java

BoundedBuffer.class: BoundedBuffer.java
	javac BoundedBuffer.java

run: Main.class
	java Main

clean:
	rm *.class

tar:
	tar -cvf CaoN.HW5.CS5600.S23.tar Makefile README Main.java BoundedBuffer.java