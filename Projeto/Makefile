CC=g++
SigmaIP=193.136.128.108

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
	
entrarsigma:
	ssh ist1103327@sigma.ist.utl.pt
	
passarficheiros:
	scp -r ../Projeto ist1103327@sigma.ist.utl.pt:~/RC
	
servertests:
	echo "$(SigmaIP) 58028 1" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report01.html
	echo "$(SigmaIP) 58028 2" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report02.html
	echo "$(SigmaIP) 58028 3" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report03.html
	echo "$(SigmaIP) 58028 4" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report04.html
	echo "$(SigmaIP) 58028 5" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report05.html
	echo "$(SigmaIP) 58028 6" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report06.html
	echo "$(SigmaIP) 58028 7" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report07.html
	echo "$(SigmaIP) 58028 8" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report08.html
	echo "$(SigmaIP) 58028 9" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report09.html
	echo "$(SigmaIP) 58028 10" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report10.html
	echo "$(SigmaIP) 58028 11" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report11.html
	echo "$(SigmaIP) 58028 12" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report12.html
	echo "$(SigmaIP) 58028 13" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report13.html
	echo "$(SigmaIP) 58028 14" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report14.html
	echo "$(SigmaIP) 58028 15" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report15.html

servertest21:
	echo "$(SigmaIP) 58028 21" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report21.html

servertest22:
	echo "$(SigmaIP) 58028 22" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report22.html

servertest23:
	echo "$(SigmaIP) 58028 23" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report23.html

servertest24:
	echo "$(SigmaIP) 58028 24" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report24.html

servertests25:
	echo "$(SigmaIP) 58028 25" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report25.html

servertest26:
	echo "$(SigmaIP) 58028 26" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report26.html

servertest27:
	echo "$(SigmaIP) 58028 27" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report27.html

servertest28:
	echo "$(SigmaIP) 58028 28" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report28.html

servertest29:
	echo "$(SigmaIP) 58028 29" | nc tejo.tecnico.ulisboa.pt 59000 > Reports/report29.html