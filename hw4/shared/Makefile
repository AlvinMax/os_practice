.PHONY: all clean

all: ../libs/shared_lib.so

../libs/shared_lib.so: sub.o
	g++ -std=c++14 -shared -o $@ $^
	rm -rf *.o

%.o: %.cpp
	g++ -std=c++14 -fPIC -c -o $@ $^

