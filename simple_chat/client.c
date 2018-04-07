#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h> // structures needed for socket
#include<netdb.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // structures needed for internet domain addresses
#include <arpa/inet.h>

#include<stdlib.h>
#define MAX 1024 // the maximum size of the data
#define PORT 8080  // defined port no.
#define SA struct sockaddr

// function for operations on the message data
void func(int sockfd)
{
	char data[MAX];
	int n;
	for(;;)  //  enter into an infinite loop
	{
	bzero(data,sizeof(data));  // initialize all the data to 0
	printf("Computer says : ");
	n=0;
	while((data[n++]=getchar())!='\n');  //  while not end of line
	write(sockfd,data,sizeof(data));  // write the data from the terminal
	bzero(data,sizeof(data));  // again reinitialize data back to 0
	read(sockfd,data,sizeof(data)); // read the data back into the buffer
	printf("From Prateek : %s",data);

 	if((strncmp(data,"quit",4))==0)  // if computer types in `quit`, exit the conversation
	{
		printf("Computer Exits Conversation...\n");
		break;
	}

	}
}


int main()
{
	//defining the socket file descriptors
	int sockfd,connfd;
	//a structure containing an internet address.
	//This structure is defined in <netinet/in.h>
	struct sockaddr_in servaddr,cli;
	sockfd=socket(AF_INET,SOCK_STREAM,0); // creating the socket

	// if socket creation failed file descriptor returned will be -1
	if(sockfd==-1)	{
		printf("socket creation failed...\n");
		exit(0); } // exit abnormally from the program
	else
  		printf("Socket successfully created..\n"); // else socket created successfully

	// initialize the serve_addr to 0
	bzero(&servaddr,sizeof(servaddr));
	// Setting the functionalities of the structure
	// setting the address family and the port no.
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(PORT);

    // connect to the socket with the given sock_fd and server address
	if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
	{ // if error then return value non-zerp
		printf("connection with the server failed...\n");
		exit(0); // exit the program abnormally
	}

	else
		printf("connected to the server..\n");

	func(sockfd);// run the message function
	close(sockfd);// close the socket message
}
