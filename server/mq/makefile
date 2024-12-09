all: publish consume

publish:publish.cc
	g++ -std=c++17 $^ -o $@  -L/usr/lib -lamqpcpp -lev -lpthread -lgflags -lspdlog -lfmt

consume:consume.cc
	g++ -std=c++17 $^ -o $@  -L/usr/lib -lamqpcpp -lev -lpthread -lgflags -lspdlog -lfmt

.PHONY:clean
clean:
	rm publish consume