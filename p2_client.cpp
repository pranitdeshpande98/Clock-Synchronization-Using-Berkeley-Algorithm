#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#define MAXELEMENTSIZE 30000
#define PORT 9991
using namespace std;


int main(int argc, char *argv[]){

//Start of Bonus assignment - Adding feature of Distributed Locking
	pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
	cout << "---------------------Start of Bonus Assignment----------------------------" << endl;
	cout <<"-----------Reading the file-----------" << endl;           
	pthread_mutex_lock(&m1);  //Acquiring the lock using mutexes
	int count_value;
	FILE *file_read = fopen("counter.txt","r+"); // Open the file in read only mode.
// Reading the file	
	if(file_read == NULL)    
	{
		cout << "Error in reading the file in read mode" << endl;
		return 0;
	}
	else
	{
		char buffer[256];
		fgets(buffer,sizeof(buffer),file_read);
		count_value = atoi(buffer);
		cout << "Current count value: " << count_value << endl;
		count_value++;
	}
	fclose(file_read);
// Writing the file	
	FILE *file_write = fopen("counter.txt","w+"); // Open the file in write only mode
	if(file_write == NULL)
	{
		cout << "Error in reading the file in writing mode" << endl;
		return 0;
	}
	else
	{
		fprintf(file_write, "%d\n",count_value);
		cout << "Updated counter value: " << count_value << endl;
	}
	fclose(file_write);
	pthread_mutex_unlock(&m1);  	//Releasing lock
	cout <<"-------------------------File is updated successfully------------------" << endl;
	cout << "---------------------End of Bonus Assignment----------------------------" << endl;
//End of Bonus Assignment

	int count = 0,socket_fd, portno = PORT;
	float local_clock = 0.0, newTime;
	srand(time(0));						
	local_clock = (rand()%10); // Randomly initialized client / slave processes local clock
	string time_to_string;
	time_to_string = to_string(local_clock);
	char msg[1025];
	strcpy(msg,time_to_string.c_str());
	struct sockaddr_in server_address;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) //Step 1 : Create the server socket,and if the returned value is less than 0 print the error.
	{
		cout << "Error in creating socket" <<endl; 
		exit(1);
	
	}
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(portno);
	if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == 0) { // Step2- Connect to socket
		char buf[1024] = {0};
		cout<<"Sending current local time to server: "<<msg<<" to server "<<endl;
		send(socket_fd,msg, strlen(msg),0); //Sending your local time to server
		while(1){
			while(count = read(socket_fd, buf, 1024) > 0 ) {
				printf("Server received local time as: %s\n", buf);
				cout<<"---------------------------------------"<<endl;
				newTime = stoi(buf);
				cout<<"Offset is :"<< newTime - local_clock<<endl;
				cout<<"The new synchronized time is :"<<newTime<<endl;
				shutdown(socket_fd, SHUT_RDWR);
				return 0;
			}
		}
	}else{
		cout << "Error in connecting to the socket" <<endl;
		exit(1);
		}
}
