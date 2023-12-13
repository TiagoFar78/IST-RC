CC=g++

all: CompileServer CompileClient

CompileServer: 
	$(CC) -std=c++17 ArgumentVerification.cpp AuctionManager.cpp Server.cpp -o Server
	
CompileClient:
	$(CC) -std=c++17 ArgumentVerification.cpp client.cpp -o Client
	
tests:
	$(CC) -std=c++17 ArgumentVerification.cpp RunTests.cpp -o RunTests
	
clean:
	rm Client
	rm Server
	rm RunTests