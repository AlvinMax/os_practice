.PHONY: all

all: static_lib.a

static_lib.a: add.o
	ar rcs ../libs/$@ $^
	rm -rf *.o

%.o: %.cpp
	g++ -std=c++14 -c -o $@ $^
