//ftclient.c
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd;
	char filename[2000],filebuffer[2000];
	struct sockaddr_in servaddr;
	
	if((fd = socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("Socket Creation failed..");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	printf("Enter textfile Name to send : \n");
	scanf("%s",filename);
	sendto(fd,filename,strlen(filename),0,(struct sockaddr*)&servaddr,sizeof(struct sockaddr));
	
	FILE *fp;
	fp = fopen(filename,"r");
	
	if(fp)
	{
		printf("Reading file Contents...\n");
		fseek(fp,0,SEEK_END);
		size_t file_size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		if(fread(filebuffer,file_size,1,fp)<=0)
		{
			printf("Unable to copy file into Buffer...\n");
			exit(1);
		}
		
	}
	else
	{
		printf("Cannot open the File...\n");
		exit(0);
	}
	
	printf("FILE CONTENTS TO SEND : %s\n",filebuffer);
	if(sendto(fd,filebuffer,strlen(filebuffer),0,(struct sockaddr*)&servaddr,sizeof(struct sockaddr))<0)
	{
		printf("File Was Not Sent...\n");
	}
	
	else
	{
		printf("File Sent\n");
	}
	fclose(fp);

}

//ftserver.c
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

#define maxlen 70000
#define mlen 100000

int main()
{
	char filename[100];
	char filebuffer[2000];
	int sd,connfd,len;
	
	for(int i=0;i<=100;i++)
	{
		filename[i] = '\0';
	}
	
	struct sockaddr_in servaddr,cliaddr;
	
	sd = socket(AF_INET,SOCK_DGRAM,0);
	
	if(sd==-1)
	{
		printf("Socket Not Created in Server\n");
		exit(0);
	}
	else
	{
		printf("Socket Created In Server\n");
	}
	
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	memset(&(servaddr.sin_zero),'\0',8);
	
	if(bind(sd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0)
	{
		printf("Not Binded...\n");
	}
	else
	{
		printf("Binded...\n");
	}
	
	len = sizeof(cliaddr);
	
	recvfrom(sd,filename,1024,0,(struct sockaddr*)&cliaddr,&len);
	printf("Name of the text file Recieved : %s\n",filename);
	FILE *fp;
	printf("Contents in Received text file : \n");
	recvfrom(sd,filebuffer,1024,0,(struct sockaddr*)&cliaddr,&len);
	printf("%s\n",filebuffer);
	int fsize = strlen(filebuffer);
	fp=fopen(filename,"w");
	
	if(fp)
	{
		fwrite(filebuffer,fsize,1,fp);
		printf("File Received Successfully..\n");
	}
	else
	{
		printf("Cannot Create Output File....\n");
	}
	
	memset(filename,'\0',sizeof(filename));
	fclose(fp);
	return(0);
	
}

//helloclient
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    // Send "Hello" to the server
    strcpy(buffer, "Hello");
    send(clientSocket, buffer, strlen(buffer), 0);
    printf("Message Sent to Server....\n");
    
    // Receive a message from the server
    recv(clientSocket, buffer, 1024, 0);
    printf("Server says: %s\n", buffer);

    return 0;
}

//helloserver
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h> // Include this header for inet_pton

int main()
{
    int welcomeSocket, newSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Create the socket
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (listen(welcomeSocket, 5) == 0)
        printf("Listening to Client....\n");
    else
        printf("Error\n");

    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    // Receive a message from the client
    recv(newSocket, buffer, 1024, 0);
    printf("Client says: %s\n", buffer);

    // Send "Hello" to the client
    strcpy(buffer, "Hello");
    send(newSocket, buffer, strlen(buffer), 0);

    return 0;
}

