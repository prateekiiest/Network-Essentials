#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

// the message buffer size
#define BUFSIZE 1024

// This function creates a random string with size specified from the given characterset
char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {  // while loop over all the character set
            int key = rand() % (int) (sizeof charset - 1);
             // choose any one of the characters from the character set
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str; // return the string created
}

// We will use this function wherever any exception gets thrown
// Not using printf in those areas, because of debugging convenience
void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char **argv) {

    time_t t;
    srand((unsigned) time(&t));
    unsigned int time_in_micros;

    int sockfd, portno, n, P, TTL, NumPackets;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    unsigned char buf[BUFSIZE];

     /* Enter the six command line arguments, otherwise terminate programm abnormally */
    if (argc != 6) {
        // Enter commands like this only
       fprintf(stderr,"usage: %s <ServerIP> <Port> <P> <TTL> <NumPackets>\n", argv[0]);
       exit(0);
    }

    hostname = argv[1]; // first argument is host_name
    portno = atoi(argv[2]);  // second argument is the port number which needs to be converted to integer from character
    P = atoi(argv[3]);  // payload as the third argument
    TTL = atoi(argv[4]); //  TTL as the 4th argument

    // TTL should be even number
    while (TTL%2!=0) {
        printf("TTL must be even number. Re enter.\n");
        scanf("%d", &TTL);
    }

    NumPackets = atoi(argv[5]);  // NumPackets  = Number of Packets to be sent

    // create the socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);/* create the socket file descriptor using
                                                 socket family = AF_INET, type =  datagram (UDP)*/

    if (sockfd < 0)
        error("ERROR opening socket");
    /*The gethostbyname() function returns a structure of  type  hostent  for
       the given host name.*/
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    struct timeval tv;
    printf("%d\n", time_in_micros);

    int i;

    int total_delay=0;
    for (i=1; i<=NumPackets; i++) {
        int offset=0;
        bzero(buf, BUFSIZE);
        printf("%d\n", i); // the numpackets

        unsigned short seq_no = i;
        seq_no = htons(seq_no); // seqno
        memcpy(&buf[offset], &seq_no, sizeof(short)+1);
        offset+=sizeof(short);

        gettimeofday(&tv,NULL);
        time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
        unsigned int timex = htonl(time_in_micros);
        memcpy(&buf[offset], &timex, sizeof(int));
        offset+=sizeof(int);

        char ttlx = (char)TTL;
        printf("Sender ttlx : %d\n", (int)ttlx);
        memcpy(&buf[offset], &ttlx, sizeof(char));
        offset+=sizeof(char);

        char random_payload[20];
        rand_string(random_payload, P+1);
        memcpy(&buf[offset], &random_payload, sizeof(random_payload));
        offset+=sizeof(random_payload);

        serverlen = sizeof(serveraddr);
        gettimeofday(&tv,NULL);
        time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
        printf("Sending time : %u\n", time_in_micros);  // time when the packet is sent

        // Have created the UDP data
	int sending_time = time_in_micros;
        // send the packet to the server
        n = sendto(sockfd, buf, 7+P, 0, &serveraddr, serverlen);
        if (n < 0)
            error("ERROR in sendto");
        // check for incoming messages from the server
        n = recvfrom(sockfd, buf, 7+P, 0, &serveraddr, &serverlen);
        if (n < 0)
            error("ERROR in recvfrom");

       // gettimeofday(&tv,NULL);
        printf("Receiving time : %u\n", time_in_micros);
        int receiving_time = time_in_micros;   // time when the packet gets received
        // get the total delay and print it
        printf("RTT delay in packet %d is : %d microseconds\n", i,receiving_time - sending_time);
        total_delay += receiving_time - sending_time;
 }
    return 0;
}
