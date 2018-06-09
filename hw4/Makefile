.PHONY: all clean

all: main

main: main.o ./libs/static_lib.a ./libs/shared_lib.so
	g++ -std=c++14 -L./libs -Wl,-rpath,. -o $@ $^ -ldl



%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^



../libs/shared_lib.so: make_shared.sh
	chmod +x $<
	./$<

../libs/loadable_lib.so: make_loadable.sh
	chmod +x $<
	./$<

../libs/static_lib.a: make_static.sh
	chmod +x $<
	./$<

clean:
	rm main

