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
#define LOCAL_PORT 3001

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b);
int packData(unsigned char *buffer, unsigned int a, unsigned int b);

int main(int argc, char *argv[]) {
	struct sockaddr_in their_addr; // connector's address information
	struct sockaddr_in my_addr; // local address information

	struct hostent *he;
	int numbytes;
	int serverPort;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int testa = 0;
	unsigned int testb = 0;
	int socketfd = 0;

	int sendRepeatTimes = 50;

	unsigned char buffer[4];

	printf("TCP client\n\n");

	if (argc != 5) {
		fprintf(stderr, "Usage: tcpClient serverName serverPort int1 int2\n");
		exit(1);
	}

	serverPort = atoi(argv[2]);
	a = (unsigned int) atoi(argv[3]);
	b = (unsigned int) atoi(argv[4]);

	//Resolv hostname to IP Address
	if ((he = gethostbyname(argv[1])) == NULL ) {  // get the host info
		herror("gethostbyname");
		exit(1);
	}
	printf("Arguments server=%s:%i NrA=%i NrB=%i\n", argv[1], serverPort, a, b);

	// create server socket
	socketfd = socket(PF_INET, SOCK_STREAM, 0);

	if (socketfd == -1) {
		fprintf(stderr, "Create Socket: Error %i\n", errno);
	} else {
		//setup transport address
		puts("Create socket!\n");
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(serverPort);
		their_addr.sin_addr = *((struct in_addr *) he->h_addr);
		memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(LOCAL_PORT);
		my_addr.sin_addr.s_addr = INADDR_ANY;
		memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

		// bind socket to  local machine
		int yes = 1;

		// reuse socket if already in used
		if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof my_addr)
				== -1) {
			fprintf(stderr, "Bind socket: Error %i\n", errno);
		} else {
			puts("Bind socket!\n");

			packData(&buffer, a, b);
			unpackData(&buffer, &testa, &testb);

			if (testa != a || testb != b) {
				fprintf(stderr, "Pack Data: wrong implementation \n");
			} else {
				puts("Pack data!\n");
			}

			if (connect(socketfd, (struct sockaddr *) &their_addr,
					sizeof their_addr) == -1) {
				fprintf(stderr, "connect to server: Error %i\n", errno);
			} else {
				puts("connected to server!\n");

				while (sendRepeatTimes--) {
					numbytes = send(socketfd, buffer, sizeof buffer, 0);
					if (numbytes == -1) {
						fprintf(stderr, "send: Error %i\n", errno);
					}
					printf("send %i Bytes\n",numbytes);
				}
			}
			close(socketfd);
			puts("close socket!\n");
			puts("tcpClient End\n\n");
		}
	}
	return 0;
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
