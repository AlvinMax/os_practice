.PHONY: all clean

all: jit_compiler

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

jit_compiler: jit_compiler.o
	g++ -std=c++17 -o jit_compiler $^

clean:
	rm -rf jit_compiler *.o


