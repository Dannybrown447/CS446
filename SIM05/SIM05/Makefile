all: SIM05

SIM05: main.cpp ReadData.o Simulator.o MemoryFunction.o
	g++ -pthread -g main.cpp ReadData.o Simulator.o MemoryFunction.o -lpthread -o SIM05
	

ReadData.o: ReadData.cpp ReadData.h
	g++ -pthread -std=c++11 -c ReadData.cpp

Simulator.o: Simulator.cpp Simulator.h
	g++ -pthread -std=c++11 -c Simulator.cpp

MemoryFunction.o: MemoryFunction.cpp MemoryFunction.h
	g++ -c MemoryFunction.cpp

clean:
	rm -rf *.o*
