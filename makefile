CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude

LFLAGS:= -lboost_system -lpthread

all: Client
	g++ -o bin/Client bin/connectionHandler.o bin/Client.o bin/Task.o $(LFLAGS)

Client: bin/connectionHandler.o bin/Client.o bin/Task.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/Task.o: src/KeyboardReader.cpp
	g++ $(CFLAGS) -o bin/Task.o src/Task.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
