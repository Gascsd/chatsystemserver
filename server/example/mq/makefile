all: publish consume

publish:publish.cc
	g++ -std=c++17 $^ -o $@  /usr/lib/libamqpcpp.so.4.3.27 -lev -lpthread -lgflags -lspdlog -lfmt

consume:consume.cc
	g++ -std=c++17 $^ -o $@  /usr/lib/libamqpcpp.so.4.3.27 -lev -lpthread -lgflags -lspdlog -lfmt

.PHONY:clean
clean:
	rm publish consume