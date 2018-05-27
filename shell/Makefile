.PHONY: all clean

all: shell

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

shell: shell.o
	g++ -std=c++17 -o shell $^

clean:
	rm -rf shell *.o


