/*
 ############################################################################
 #                                                                          #
 # Copyright TU-Berlin, 2011                                                #
 # Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
 #                                                                          #
 ############################################################################
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_LENGTH 100
#define BACKLOG		20

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b);
int packData(unsigned char *buffer, unsigned int a, unsigned int b);
unsigned int gcd(unsigned int u, unsigned int v);
char* hexToCharIP(struct in_addr addrIP);
int setupTCPSocket(int port);

int main(int argc, char *argv[]) {
	struct sockaddr_in their_addr; // connector's address information
	struct sockaddr_in my_addr; // local address information
	socklen_t addr_size;

	struct hostent *he;
	int numbytes;
	int tcpPort;
	int udpPort;
	int socketfdTCP = 0;
	int socketfdUDP = 0;
	int incommingsocketfdTCP = 0;
	unsigned int a = 0, b = 0;

	int sendRepeatTimes = 50;

	unsigned char readTcpBuffer[4];

	printf("TCP/UDP Server \n\n");

	if (argc != 3) {
		fprintf(stderr, "Usage: tcpServer tcpPort udpPort\n");
		exit(1);
	}

	tcpPort = atoi(argv[1]);
	udpPort = atoi(argv[2]);

	printf("Arguments TCP-Port=%i UDP-Port=%i\n", tcpPort, udpPort);

	socketfdTCP = setupTCPSocket(tcpPort);


	while (1) {
		// wait & accept incomming queries
		incommingsocketfdTCP = accept(socketfdTCP,
				(struct sockaddr *) &their_addr, &addr_size);
		if (incommingsocketfdTCP == -1) {
			fprintf(stderr, "Accept: Error %i\n", errno);
		}
		printf("Got message from %s:%i\n",
				hexToCharIP(their_addr.sin_addr), their_addr.sin_port);
		// read
		read(incommingsocketfdTCP, readTcpBuffer, sizeof readTcpBuffer);

		unpackData(readTcpBuffer, &a, &b);
		printf("gcd of %i and %i = %i\n",a,b,gcd(a,b));


		// close
		//close(incommingsocketfdTCP);

	}
	close(socketfdTCP);
	puts("close socket!\n");
	puts("TCP/UDP Server End\n\n");

	return 0;
}

int setupTCPSocket(int port) {
	int socketfdTCP = 0;
	struct sockaddr_in my_addr; // local address information
// create server socket for tcp
	socketfdTCP = socket(PF_INET, SOCK_STREAM, 0);

	if (socketfdTCP == -1) {
		fprintf(stderr, "Create Socket: Error %i\n", errno);
	} else {
		puts("Create socket for TCP!\n");
		//setup transport address
		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(port);
		my_addr.sin_addr.s_addr = INADDR_ANY;
		memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

		// bind socket to  local machine
		int yes = 1;

		// reuse socket if already in used
		if (setsockopt(socketfdTCP, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(socketfdTCP, (struct sockaddr *) &my_addr, sizeof my_addr)
				== -1) {
			fprintf(stderr, "Bind socket: Error %i\n", errno);
		} else {
			puts("Bind socket for TCP!\n");

		}
		// listen for incomming requests
		if(listen(socketfdTCP, BACKLOG)==-1)
			{
			fprintf(stderr, "listen: Error %i\n", errno);
			}
		{
			puts("listen on socket for TCP!\n");
		}
	}
	return socketfdTCP;
}

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b) {
	*a = (buffer[0] << 8) | buffer[1];
	*b = (buffer[2] << 8) | buffer[3];
}

int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
	buffer[0] = (a & 0xff00) >> 8;
	buffer[1] = a & 0xff;
	buffer[2] = (b & 0xff00) >> 8;
	buffer[3] = b & 0xff;
	return -1;
}

unsigned int gcd(unsigned int u, unsigned int v) {
	// simple cases (termination)
	if (u == v)
		return u;
	if (u == 0)
		return v;
	if (v == 0)
		return u;

	// look for factors of 2
	if (~u & 1) // u is even
		if (v & 1) // v is odd
			return gcd(u >> 1, v);
		else
			// both u and v are even
			return gcd(u >> 1, v >> 1) << 1;
	if (~v & 1) // u is odd, v is even
		return gcd(u, v >> 1);

	// reduce larger argument
	if (u > v)
		return gcd((u - v) >> 1, v);
	return gcd((v - u) >> 1, u);
}

char* hexToCharIP(struct in_addr addrIP) {
	char* ip;
	unsigned int intIP;
	memcpy(&intIP, &addrIP, sizeof(unsigned int));
	int a = (intIP >> 24) & 0xFF;
	int b = (intIP >> 16) & 0xFF;
	int c = (intIP >> 8) & 0xFF;
	int d = intIP & 0xFF;
	if ((ip = (char*) malloc(16 * sizeof(char))) == NULL ) {
		return NULL ;
	}
	sprintf(ip, "%d.%d.%d.%d", d, c, b, a);
	return ip;
}
