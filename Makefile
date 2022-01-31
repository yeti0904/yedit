CC  = g++
src = $(wildcard src/*.cc)
arg = -std=c++17 -lncurses -lpthread -I./lib
out = bin/yedit

build:
	mkdir -p bin
	$(CC) $(src) -s $(arg) -o $(out)

debug:
	mkdir -p bin
	$(CC) $(src) -g $(arg) -o $(out)