all: program1 program2

program1: client.cpp
	g++ -o client -std=c++11 client.cpp sha1.hpp hashstore.cpp rle.cpp

program2: server.cpp
	g++ -o server -std=c++11 server.cpp chunkstore.cpp rle.cpp
