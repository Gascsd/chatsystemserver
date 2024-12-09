all: publish consume

publish:publish.cc
	g++ -std=c++17 $^ -o $@  -L/usr/lib -lamqpcpp -lev -lpthread

consume:consume.cc
	g++ -std=c++17 $^ -o $@  -L/usr/lib -lamqpcpp -lev -lpthread