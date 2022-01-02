CC  = g++
src = $(wildcard src/*.cc)
arg = -std=c++17 -lncurses
out = bin/yedit

build:
	$(CC) $(src) -s $(arg) -o $(out)
