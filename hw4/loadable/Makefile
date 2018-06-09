.PHONY: all clean

all: ../libs/loadable_lib.so

../libs/loadable_lib.so: mul.o
	g++ -std=c++14 -shared -o $@ $^
	rm -rf *.o

%.o: %.cpp
	g++ -std=c++14 -fPIC -c -o $@ $^

