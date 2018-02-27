#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<stdlib.h>
#define MAX 180
#define PORT 8080
#define SA struct sockaddr

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
	read(sockfd,data,sizeof(data));
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
	int sockfd,connfd;
	struct sockaddr_in servaddr,cli;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

        // Creating socket
	if(sockfd==-1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}

	else
  		printf("Socket successfully created..\n");


	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servaddr.sin_port=htons(PORT);



	if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
	{
 		 // connection failed with server
		printf("connection with the server failed...\n");
		exit(0);
	}

	else
		printf("connected to the server..\n");

	func(sockfd);// run the message function
	close(sockfd);// close the socket message
}
