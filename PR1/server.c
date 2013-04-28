

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h> 
#include <sys/select.h> 
#include <fcntl.h>
#include <ctype.h>

#define MAX_BUFFER_LENGTH 100
#define BACKLOG		20

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

unsigned int getGCD(unsigned int a, unsigned int b) {
  return gcd(a, b);
}

void unpackData(unsigned char* buffer, unsigned int* a, unsigned int* b) {
  *a = (buffer[0]<<8)|buffer[1];
  *b = (buffer[2]<<8)|buffer[3];
}

void readBuff(unsigned char* buffer) {
    unsigned int* a, *b;
    a = (unsigned int*) malloc(1);
    b = (unsigned int*) malloc(2);

    unpackData(buffer, a, b);
    printf("\n Received values %d and %d.\n", *a, *b);
    unsigned int gcd = getGCD(*a, *b);
    printf("\n GCD is %d \n", gcd);

    free (a);
    free (b);
}

int main(int argc, char *argv[])
{
  printf("\n TCP/UDP server \n\n");
  
  // Check argument count.
  if (argc != 4) {
      fprintf(stderr,"Usage: server tcpPort udpPort \n");
      exit(1);
  }

  unsigned int tcpPort, udpPort;
  char* serverAddr;
  serverAddr = (char*) malloc(12);
  serverAddr = argv[1];
  tcpPort = atoi(argv[2]);
  udpPort = atoi(argv[3]);
  printf(" \n Using server address %s \n", serverAddr);
  printf(" \n Using TCP Port %d \n", tcpPort);
  printf(" \n Using UDP Port %d \n", udpPort);

  /****************************************************************************
   *  TCP Server
   * *************************************************************************/
  // Create TCP socket.
  int tcp_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_sd <= 0){
    printf("\n ERROR: Failed to create TCP socket. \n");
    exit(1);
  }

  // Create TCP address.
  struct sockaddr_in tcp_address;
  memset(&tcp_address, '\0', sizeof(tcp_address));
  tcp_address.sin_family = AF_INET;
  tcp_address.sin_port = htons(tcpPort); 
  tcp_address.sin_addr.s_addr = inet_addr(serverAddr);
  tcp_address.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_stat;
  bind_stat = bind(tcp_sd, (struct sockaddr*)&tcp_address, sizeof(tcp_address));
  if (bind_stat == -1) {
    printf("\n ERROR: bind TCP socket failed. \n");
    exit(1);
  } 

  if (listen(tcp_sd, BACKLOG) == -1) {
    printf("\n Listen on TCP socket failed.\n");
    exit(1);
  } else {
    printf("\n Listening on TCP socket.\n");
  }

  /****************************************************************************
   *  UDP Server
   * *************************************************************************/
  // Create UDP socket.
  int udp_sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_sd <= 0){
    printf("\n ERROR: Failed to create UDP socket. \n");
    exit(1);
  }

  // Create UDP address.
  struct sockaddr_in udp_address;
  memset(&udp_address, '\0', sizeof(udp_address));
  udp_address.sin_family = AF_INET;
  udp_address.sin_port = htons(udpPort); 
  udp_address.sin_addr.s_addr = inet_addr(serverAddr);
  udp_address.sin_addr.s_addr = htonl(INADDR_ANY);
  //int len_addr = sizeof()

  bind_stat = bind(udp_sd, (struct sockaddr*)&udp_address, sizeof(udp_address));
  if (bind_stat == -1) {
    printf("\n ERROR: bind failed. \n");
    exit(1);
  } 
  printf("\n Accepting data on UDP port.\n");

  fd_set readfds;
  int max_sd, rc;
  struct timeval timeout;
  timeout.tv_sec = 3 * 60;
  timeout.tv_usec = 0;
  if (tcp_sd > udp_sd)  max_sd = tcp_sd;
  else                  max_sd = udp_sd;


  while (1) {
    // Set FD sets.
    FD_SET(tcp_sd, &readfds);
    FD_SET(udp_sd, &readfds);
    rc = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
    if (rc < 0) {
      printf("\n ERROR: select() failed. \n");
      exit(1);
    }

    if (rc == 0) {
      printf("\n select() timed out. End program.\n");
      exit(1);
    }
    
    int i = 0;
    for (i = 0; i <= max_sd; i++) {
      if (FD_ISSET(i, &readfds)) {
        
        // Receive on UDP port
        if (i == udp_sd) {
          char recvBuff[100];
          memset(recvBuff, '0', sizeof(recvBuff)); 

          unsigned int maxPackSize = sizeof(recvBuff);
          struct sockaddr_in from;
          socklen_t fromLength = sizeof(from);

          int receivedBytes = recvfrom(
              udp_sd, 
              (char*) recvBuff, 
              maxPackSize, 
              0, 
              (struct sockaddr*) &from, 
              &fromLength);

          if (receivedBytes <= 0) break;
          readBuff(recvBuff);

        // Receive on TCP port
        } else if (i == tcp_sd) {
          char recvBuff[100];
          memset(recvBuff, '0', sizeof(recvBuff)); 
          unsigned int maxPackSize = sizeof(recvBuff);
          int incoming_tcp_sd;
          struct sockaddr_in from;
          socklen_t fromLength = sizeof(from);
          incoming_tcp_sd = accept(
              tcp_sd,
              (struct sockaddr*) &from, 
              &fromLength);
          read(incoming_tcp_sd, recvBuff, sizeof recvBuff);
          readBuff(recvBuff);
        }
      }
    } // End For Loop through sockets
  }   // End While
}     // End main()
  
