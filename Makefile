all: a1p3 a1p2

a1p3: a1p3.o
	g++ a1p3.cpp -o a1p3

a1p2: a1p2.o
	g++ a1p2.cpp -o a1p2

clean:
	rm -f a1p2 a1p3 *.o out1 outA outB

tar:
	tar -cvf abbas-a1.tar *.cpp Makefile bash.man a1p3.data report.pdf