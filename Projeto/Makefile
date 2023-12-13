CC=g++
SigmaIP=193.136.128.103

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
	echo "$(SigmaIP) 58028 16" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report16.html
	echo "$(SigmaIP) 58028 17" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report17.html
	echo "$(SigmaIP) 58028 18" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report18.html
	echo "$(SigmaIP) 58028 19" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report19.html
	echo "$(SigmaIP) 58028 20" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report20.html
	echo "$(SigmaIP) 58028 21" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report21.html
	echo "$(SigmaIP) 58028 22" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report22.html
	echo "$(SigmaIP) 58028 23" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report23.html
	echo "$(SigmaIP) 58028 24" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report24.html
	echo "$(SigmaIP) 58028 25" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report25.html
	echo "$(SigmaIP) 58028 26" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report26.html
	echo "$(SigmaIP) 58028 27" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report27.html
	echo "$(SigmaIP) 58028 28" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report28.html
	echo "$(SigmaIP) 58028 29" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report29.html