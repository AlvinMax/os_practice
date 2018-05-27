.PHONY: all clean

all: mygrep

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

mygrep: mygrep.o
	g++ -std=c++17 -o mygrep $^

clean:
	rm -rf mygrep *.o


