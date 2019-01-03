CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: EchoClient
	g++ -o bin/BGSclient bin/connectionHandler.o bin/echoClient.o bin/clientToServer.o bin/serverToClient.o $(LDFLAGS)

EchoClient: bin/connectionHandler.o bin/echoClient.o bin/clientToServer.o bin/serverToClient.o

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/clientToServer.o: src/clientToServer.cpp
	g++ $(CFLAGS) -o bin/clientToServer.o src/clientToServer.cpp

bin/serverToClient.o: src/serverToClient.cpp
	g++ $(CFLAGS) -o bin/serverToClient.o src/serverToClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
