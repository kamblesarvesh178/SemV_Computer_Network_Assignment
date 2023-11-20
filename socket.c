//tcpclient.c
// Client side C/C++ program to demonstrate Socket 
// programming 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#define PORT 8080 
int main(int argc, char * argv[]) { 
	int status, valread, client_fd;
	char* msg;	
	struct sockaddr_in serv_addr;
	printf("Type message for the server : ");
   	scanf("%[^\n]s",msg);	
	char buffer[1024] = { 0 };
	
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);	

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	
	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	
	send(client_fd, msg, strlen(msg), 0);
	printf("Received Message from server\n");
	valread = read(client_fd, buffer, 1024);
	printf("%s\n", buffer);close(client_fd);
	return 0;
}


//tcpserver.c
// Server side C/C++ program to demonstrate Socket 
// programming  int argc, char const* argv[]
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#define PORT 8080 
int main() {  
	int server_fd, new_socket, valread;   
	struct sockaddr_in address;int opt = 1;int addrlen = sizeof(address);
   	char buffer[1024] = { 0 };
   	char* msg;
   	printf("Type message for the client : ");
   	scanf("%[^\n]s", msg);
   	
   	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   		perror("Socket Failed"); exit(EXIT_FAILURE); 
   	}
   	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
   		perror("setsockopt");
   		exit(EXIT_FAILURE);
   	}
   	
   	address.sin_family = AF_INET;
   	address.sin_addr.s_addr = INADDR_ANY;
   	address.sin_port = htons(PORT);
   
   	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {perror("bind failed");
 		exit(EXIT_FAILURE);
 	}
 	
 	if (listen(server_fd, 3) < 0) {
 		perror("listen");exit(EXIT_FAILURE);
   	}
   	
   	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))< 0) {
   		perror("accept");
   		exit(EXIT_FAILURE);
   	}
   
   	valread = read(new_socket, buffer, 1024);
   	printf("%s\n", buffer);
   	send(new_socket, msg, strlen(msg), 0);
   	//printf("%d\n",new_socket);
   	printf("Sending....\n");
   	printf("Message sent successfully from server to client\n");
   	close(new_socket);shutdown(server_fd, SHUT_RDWR);return 0;}


//udpclient.c
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include <unistd.h> 
#define MAX 1024
#define PORT 8080
int main() {
	
	int network_socket;
	char buffer[MAX];
	const char *hello = "Hello from client!";
	
	// creation of client socket
	network_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(network_socket < 0)
	{
		perror("Socket creation failed!");
		exit(EXIT_FAILURE);
	}
	
	// define server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	int n;
	socklen_t len;
	
	// sendto function in UDP sends the hello message to server
	sendto(network_socket,(const char *)hello,strlen(hello),MSG_CONFIRM,(const struct sockaddr *)&server_address, sizeof(server_address));
	
	printf("Message sent to server!");
	
	len = sizeof(server_address);
	
	// recvfrom recieves the message from server
	n = recvfrom(network_socket,buffer,MAX,MSG_WAITALL,(struct sockaddr *)&server_address, &len);
	
	buffer[n] = '\0';
	printf("\nServer: %s\n",buffer);
	
	close(network_socket);
	return 0;
}


//udpserver.c
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include <unistd.h> 
#define MAX 1024
#define PORT 8080

int main() {
	int server_socket;
	char buffer[MAX];
	const char *hello = "Hello from server!";
	
	// creation of server socket
	server_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(server_socket < 0)
	{
		perror("Error in creating socket!");
		exit(EXIT_FAILURE);
	}
	
	//set memory for the client socket address
	struct sockaddr_in client_adddress;
	memset(&client_adddress,0,sizeof(client_adddress));
	
	// specify address of server socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	// bind the server socket with the available client socket
	bind(server_socket,(const struct sockaddr *)&server_address,sizeof(server_address));
	
	int n;
	socklen_t len;
	len = sizeof(client_adddress);
	
	// recvfrom recieves the hello message from socket
	n = recvfrom(server_socket,(char *)buffer,MAX,MSG_WAITALL,(struct sockaddr *)&client_adddress,&len);
	buffer[n] = '\0';
	printf("Client: %s\n",buffer);
	sendto(server_socket,(const char *)hello,strlen(hello),MSG_CONFIRM,(const struct sockaddr *)&client_adddress,sizeof(client_adddress));
	printf("\nMessage Sent to client\n");
	close(server_socket);
	return 0;
}
