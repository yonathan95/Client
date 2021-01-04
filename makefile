CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude

LFLAGS:= -lboost_system -lpthread

all: BGRSclient
	g++ -o bin/BGRSclient bin/connectionHandler.o bin/Client.o bin/KeyboardReader.o bin/SocketReader.o $(LFLAGS)

BGRSclient: bin/connectionHandler.o bin/Client.o bin/KeyboardReader.o bin/SocketReader.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/KeyboardReader.o: src/KeyboardReader.cpp
	g++ $(CFLAGS) -o bin/KeyboardReader.o src/KeyboardReader.cpp

bin/SocketReader.o: src/SocketReader.cpp
	g++ $(CFLAGS) -o bin/SocketReader.o src/SocketReader.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
