all: SIM01

SIM01: main.cpp ReadData.o 
	g++ -g main.cpp ReadData.o -o SIM01

ReadData.o: ReadData.cpp ReadData.h
	g++ -std=c++11 -c ReadData.cpp

clean:
	rm -rf *.o*
