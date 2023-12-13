CC=g++
SigmaIP=193.136.128.109

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
	
servertests:
	echo "$(SigmaIP) 58028 1" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report1.html
	echo "$(SigmaIP) 58028 2" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report2.html
	echo "$(SigmaIP) 58028 3" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report3.html
	echo "$(SigmaIP) 58028 4" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report4.html
	echo "$(SigmaIP) 58028 5" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report5.html
	echo "$(SigmaIP) 58028 6" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report6.html
	echo "$(SigmaIP) 58028 7" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report7.html
	echo "$(SigmaIP) 58028 8" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report8.html
	echo "$(SigmaIP) 58028 9" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report9.html
	echo "$(SigmaIP) 58028 10" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report10.html
	echo "$(SigmaIP) 58028 11" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report11.html
	echo "$(SigmaIP) 58028 12" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report12.html
	echo "$(SigmaIP) 58028 13" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report13.html
	echo "$(SigmaIP) 58028 14" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report14.html
	echo "$(SigmaIP) 58028 15" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report15.html
	echo "The next tests must be done manually"
	
	#login 111111 aaaaaaaa
	#login 222222 bbbbbbbb
	#login 333333 cccccccc
	#login 444444 dddddddd
	#"$(SigmaIP) 58028 21" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report21.html
	#"$(SigmaIP) 58028 22" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report22.html
	#"$(SigmaIP) 58028 23" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report23.html
	#"$(SigmaIP) 58028 24" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report24.html