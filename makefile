SHELL = /bin/sh

CXX = g++

#compiler flags:
CFLAGS = -std=c++11 -g -Wall
SRC_DIR = ./src/
INCLUDE_DIR = ./include/

objects = graph.o neural_net.o main.o

executable_name = neural_net

default: neural_net


# all: graph.o neural_net.o  main.o
# 	$(CXX) $(CFLAGS) -o all graph.o neural_net.o main.o 

all: $(objects)
	$(CXX) $(CFLAGS) -o all $(objects)
	mkdir -p bin
	mv all bin/$(executable_name)

graph.o: $(SRC_DIR)graph.cpp $(INCLUDE_DIR)graph.h
	$(CXX) $(CFLAGS) -c $(SRC_DIR)graph.cpp

neural_net.o: $(SRC_DIR)neural_net.cpp $(INCLUDE_DIR)neural_net.h
	$(CXX) $(CFLAGS) -c $(SRC_DIR)neural_net.cpp

main.o: ./src/main.cpp
	$(CXX) $(CFLAGS) -c $(SRC_DIR)main.cpp


clean:
	rm bin/$(executable_name) $(objects) 
