all: discovery registrant

discovery : discovery.cc main.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lspdlog -lfmt -letcd-cpp-api -lcpprest -lgflags -lssl -lcrypto -lprotobuf -lleveldb

registrant : registrant.cc main.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lspdlog -lfmt -letcd-cpp-api -lcpprest -lgflags -lssl -lcrypto -lprotobuf -lleveldb 

	
test:test.cc
	g++ -std=c++17 test.cc -o test -lbrpc -lgflags -lssl -lcrypto -lprotobuf -lleveldb -lspdlog -lfmt -ljsoncpp