# Makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS = -lncurses
endif
ifeq ($(UNAME_S),Darwin)
    LIBS = -lncurses
endif
ifeq ($(OS),Windows_NT)
    LIBS = -lpdcurses
endif

tetris: tetris.cpp
	$(CXX) $(CXXFLAGS) -o tetris tetris.cpp $(LIBS)

clean:
	rm -f tetris tetris.exe

install: tetris
	cp tetris /usr/local/bin/

.PHONY: clean install
