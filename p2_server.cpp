#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string>
#include <bits/stdc++.h>
#define PORT 9991
#define MAXELEMENTSIZE 30000

using namespace std;
int a[MAXELEMENTSIZE],total_number_of_processes = 0,flag = 0,counter;
int current_process_count=0;
float offset = 0.0, newTime = 0.0,local_clock = 0.0;
string time_to_change;

void Berk_Algorithm(){ // Implementation of Berkeley Algorithm
	int sum = 0;
	cout << "\n----------Server Details----------" << endl;
	cout<<"My current local time is: "<<local_clock<<endl;
	for(int index =0; index < total_number_of_processes; index++ ){
		sum = sum + (a[index]-local_clock);
	}
	offset = sum / (total_number_of_processes + 1);  // Offset(Drift Calculation)
	newTime = local_clock + offset; // Average Calculation
	cout << "Clock offset: " << offset << endl;
	local_clock = local_clock + offset;
	cout << "The new synchronized time of server is: " << newTime << endl;	
}

void* read_write(void* arg){
    	char buffer[1024] = {0};
	int count;
	pthread_detach(pthread_self());
	int sock = *((int*)arg);
	count = read(sock, buffer, 1024);
	printf("Server received the local time of process %d as: %s\n", current_process_count+1,buffer);
	
	int x = std::stoi((string)buffer);
	a[current_process_count] = x;
	current_process_count++;
	cout<<"The count of current processes is: "<<current_process_count<<endl;
	if(current_process_count == total_number_of_processes){
		Berk_Algorithm();
		flag = 1;
	}
		
	cout<<"----------------------------------------"<<endl;

	while(flag != 1);
	time_to_change = to_string(newTime);
	char buff[1025];
	strcpy(buff, time_to_change.c_str());
	write(sock,buff, strlen(buff)); // Write buffer back to the socket
	close(sock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	cout<<"Please enter the number of total number of processes you want to connect with the master daemon:\t";
	cin>>total_number_of_processes;
	
	srand(time(0));						
	local_clock = (rand()%25)+5; // Randomly initialzed server local clock
	int socket_fd, client_sockfd, portno = PORT;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);  // Step 1 : Create the server socket,and if the returned value is less than 0 print the error.
	if (socket_fd < 0)
	{
		cout << "Error in creating the socket " <<endl;
		exit(1);
	
	}	
	
	struct sockaddr_in server_address, client_address;
	bzero((char *) &server_address, sizeof(server_address));
// Citation: This code is referred from Advanced Operating System class for creating socket communication in C.  
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);
	if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) { //Step 2 : Bind the socket, and if the returned value is less than 0 print the error.
		cout <<"Error in binding the socket " << endl;
		exit(1);
	}
	listen(socket_fd,5); //Step 3: Listen to the socket,  and if the returned value is less than 0 print the error.
	int client_length = sizeof(client_address);
	counter = 0;
	pthread_t t1[MAXELEMENTSIZE];
	while(1){
		client_sockfd = accept(socket_fd, (struct sockaddr *) &client_address,(socklen_t*)&client_length); // Accept the incoming conncection from the client using accept system call
		pthread_create(&t1[counter], 0, read_write, (void*)&client_sockfd);     // Create a new thread for process.
		printf("Connecting with machine: %d \n",counter+1);
		counter++;
	}
	return 0;
}

