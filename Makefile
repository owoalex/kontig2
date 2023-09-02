CC = g++
CFLAGS = -Wall -g -std=c++23 -O3 -march=native
PACKAGE = `pkg-config --cflags`
LIBS = `pkg-config`
INC=-I./include

default:	main

main:	src/Main.cpp
	$(CC) $(PACKAGE) $(CFLAGS) $(INC) -o build/kontig src/*.cpp src/**/*.cpp

clean:
	rm -r build/*
