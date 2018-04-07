#include<stdio.h>
#include<netinet/in.h> // structures needed for internet domain addresses
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h> // structures needed for internet domain addresses
#include<netdb.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1024 // the maximum size of the data
#define PORT 8080 // defined port no.
#define SA struct sockaddr

// function for operations on the message data
void msg(int sockfd){
	char data[MAX];  // data array
	int n;

	for(;;)  // infinite loop
	{
 		bzero(data,MAX);  // initialize all the data to 0
 		read(sockfd,data,sizeof(data));  //  read the data
	 	// CLient =  Computer
        //  Server = Prateek
        // CLient prints the message data
		printf("Computer says: %s\t", data);
 		bzero(data,MAX);
		n=0;
		// Until the user enters into a new line
		while((data[n++]=getchar())!= '\n');
		// Write from the server side
			write(sockfd,data,sizeof(data));
        // if data = 'quit'
    	if((strncmp("quit",data,4))==0)//  if the server/ client says quit, exit
		{
                    // if data == quit exit the conversation
			printf("Prateek Exits conversation...\n");
			break;
		}
 	}

}





int main()

{
    // the socket file descriptors defined
	int sockfd,connfd,len;
 	//a structure containing an internet address.
	//This structure is defined in <netinet/in.h>
	struct sockaddr_in servaddr,cli;

	sockfd = socket(AF_INET, SOCK_STREAM,0); // creating the socket
	if(sockfd==-1) // if socket creation failed file descriptor returned will be -1
	{	printf("socket creation failed...\n");
		exit(0); // exit the program abnormally
	}
	else
		printf("Socket successfully created..\n");

	// initialize the serve_addr to 0
	bzero(&servaddr,sizeof(servaddr));
	// Setting the functionalities of the structure
	// setting the address family and the port no.
 	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);

	if((listen(sockfd,5))!=0){ // listen to the socketfd for incoming c
		printf("Listen failed...\n");
		exit(0);// exit the program abnormally
	}

	else
		printf("Prateek is listening..\n");


	len=sizeof(cli);
	// extracts  the  first   connection
    //  request  on  the queue of pending connections for the listening socket
	connfd=accept(sockfd,(SA *)&cli,&len);
	if(connfd<0)	{
		printf("Acceptance failed ...\n");
		exit(0);
	}
	else
		printf("Acccepting the client...\n");

	msg(connfd);

	close(sockfd);

	}
