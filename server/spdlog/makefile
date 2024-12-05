all: sync async main

main:main.cc
	g++ -o $@ $^ -std=c++17 -lfmt -lspdlog -lgflags
sync:sync.cc
	g++ -o $@ $^ -std=c++17 -lfmt -lspdlog
async:async.cc
	g++ -o $@ $^ -std=c++17 -lfmt -lspdlog