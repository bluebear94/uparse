CXX=g++
CXXFLAGS=--std=c++17 -Wall -Werror -pedantic -g
# CXXFLAGS=-std=c++17 -Wall -Werror -pdantic -O3

all: uparse.o test

uparse.o: uparse.cpp uparse.h
	$(CXX) $(CXXFLAGS) -c uparse.cpp -o uparse.o

test: test.cpp uparse.o
	$(CXX) $(CXXFLAGS) test.cpp uparse.o -o test
