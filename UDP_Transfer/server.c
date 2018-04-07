#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// the message buffer size
#define BUFSIZE 1024


int main(int argc, char **argv) {
	int sockfd; // the socket file descriptor to be used
	int portno; // the port no. to be used
	int clientlen;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	struct hostent *hostp;
	char buf[BUFSIZE]; // creating the message array buf of size 1024
	char *hostaddrp;
	int optval; // to be used while setting the socket options
	int n;

	if (argc != 2) { // if user-command exception, show the appropriate command usage
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	portno = atoi(argv[1]); // portno taken from the command line

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); /* create the socket file descriptor using
                                                 socket family = AF_INET, type =  datagram (UDP)*/

	if (sockfd < 0) // from manual page if the sockfd return value = 0, its an error
		printf("ERROR opening socket");
        // else
	optval = 1;
        // set options on the socket
        // from manual the 2nd argument (level) should be set to SOL_SOCKET to manipulate options for the socket
	//int getsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);

        // bind the socket = serveraddr assigned to socket
        //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		printf("ERROR on binding"); // on error bind returns  -1

    clientlen = sizeof(clientaddr);
	while (1) {
		/*In this while loop two things will be done
		 1> Recieve message from the other side , read the ttl value
		 2> Decrement the ttl value and send the updated messgage back*/


		// initializing the whole buf to 0 of size = buffsize
		bzero(buf, BUFSIZE);

		/*ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
		Receive messages from a socket. Places the received message into `buf`*/
		n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
		if (n < 0)   /* printf in receiving message */
			printf("ERROR in recvfrom"); // if error, then n = -1

		/*The  gethostbyaddr()  function  returns a structure of type hostent for
       the given host address addr of length len and address type type.
	   */
		hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		if (hostp == NULL) // if on error, the return type will be NULL
			printf("ERROR on getting host name");

		/* The inet_ntoa() function converts the Internet host address  in,  given
       		in  network  byte  order,  to a string in IPv4 dotted-decimal notation.
    	*/
		hostaddrp = inet_ntoa(clientaddr.sin_addr);
		if (hostaddrp == NULL)
			printf("ERROR on inet_ntoa\n");
		// print the received datagram message
		printf("server received datagram from %s (%s)\n", hostp->h_name, hostaddrp);

		// store the ttl value in a variable
		int ttlx = (int)buf[6];
		printf("Received : %d\n", ttlx);
		ttlx = ttlx-1; // decrement the ttl value
		// the new decremented ttl value is updated to the buffer message
		buf[6] = (char)ttlx;

		printf("Sent : %d\n", ttlx);
		printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);

		// send the message after all the modifications done
/*		ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);*/


		n = sendto(sockfd, buf, n, 0, (struct sockaddr *) &clientaddr, clientlen);
		if (n < 0)
			printf("ERROR in sendto");
	}
}
