CFLAGS = -I../../../odb/ -I../../../common -I./ 
main : main.cc message-odb.cxx
	g++ -std=c++17 -o $@ $^ -lodb-mysql $(CFLAGS) -lodb -lodb-boost -lspdlog -lfmt -lgflags -g