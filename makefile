SHELL = /bin/sh

CXX = g++

#compiler flags:
CFLAGS = -std=c++11 -g -Wall

default: all

all: graph.o neural_net.o  main.o
	$(CXX) $(CFLAGS) -o all graph.o neural_net.o main.o 

graph.o: graph.cpp graph.h
	$(CXX) $(CFLAGS) -c graph.cpp

NeuralNet.o: neural_net.cpp neural_net.h
	$(CXX) $(CFLAGS) -c NeuralNet.cpp

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	$(RM) *.o
