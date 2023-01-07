CFLAGS=-o
compileAll: p2_server p2_client

p2_server:p2_server.cpp
	g++ -o p2_server p2_server.cpp -lpthread
	

p2_client:p2_client.cpp
	g++ -o p2_client p2_client.cpp

p2_serverRun:
	./p2_server

p2_clientRun:
	./p2_client
		
clean:
	rm -rf *.o p2_server p2_client
