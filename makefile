SHELL = /bin/sh

CXX = g++

#compiler flags:
CFLAGS = -std=c++11 -g -Wall
SRC_DIR = ./src
INCLUDE_DIR = ./include

objects = graph.o neural_net.o main.o

executable_name = neural_net

default: neural_net


# all: graph.o neural_net.o  main.o
# 	$(CXX) $(CFLAGS) -o all graph.o neural_net.o main.o 

all: $(objects)
	$(CXX) $(CFLAGS) -o all $(objects)

graph.o: ./src/graph.cpp ./include/graph.h
	$(CXX) $(CFLAGS) -c ./src/graph.cpp

neural_net.o: ./src/neural_net.cpp ./include/neural_net.h
	$(CXX) $(CFLAGS) -c ./src/neural_net.cpp

main.o: ./src/main.cpp
	$(CXX) $(CFLAGS) -c ./src/main.cpp

clean:
	rm all $(objects) 
