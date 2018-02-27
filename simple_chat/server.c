#include<stdio.h>
#include<netinet/in.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>

#define MAX 180
#define PORT 8080
#define SA struct sockaddr

void msg(int sockfd)
{
	char data[MAX];

	int n;
// int flag = 0 ;

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

	int sockfd,connfd,len;

	struct sockaddr_in servaddr,cli;



        int new_socket;

        sockfd = socket(AF_INET, SOCK_STREAM,0);  // get the socket file descriptor
	if(sockfd==-1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	bzero(&servaddr,sizeof(servaddr));
 	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);




	if((listen(sockfd,5))!=0)
	{
		printf("Listen failed...\n");
		exit(0);
	}

	else
		printf("Prateek is listening..\n");


	len=sizeof(cli);
	connfd=accept(sockfd,(SA *)&cli,&len);
	if(connfd<0)
	{
		printf("Acceptance failed ...\n");
		exit(0);
	}

	else
		printf("Acccepting the client...\n");

	msg(connfd);

    	//pthread_create( &tid, NULL, msg, new_socket);



	 int flag = 1;
   	 while(flag) {
     		 char dat[256];
     		 bzero(dat, MAX);
       		 scanf("%s", dat);
       		 send(new_socket , dat , strlen(dat) , 0 );
	if (dat[0]=='a') {
           flag=0;

       }
    }






	close(sockfd);

	}
