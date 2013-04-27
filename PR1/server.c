

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int getGCD(int a, int b) {
  if (a > b) {
    if (a % b == 0) {
      return b;
    } else {
      b = a % b;
      getGCD(a, b);
    }
  } else {
    if (b % a == 0) {
      return a;
    } else {
      a = b % a;
      getGCD(a, b);
    }
  }
}

void unpackData(unsigned char* buffer, unsigned int* a, unsigned int* b) {
  *a = (buffer[0]<<8)|buffer[1];
  *b = (buffer[2]<<8)|buffer[3];
  printf("\n Unpacked values are:   %d  %d \n", *a, *b);
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
  /*
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 

  char sendBuff[1025];

  listenfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(tcpPort); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 

  while(1)
  {
    printf("\n Accepting conns\n");
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

    close(connfd);
    sleep(1);
  }
  */

  /****************************************************************************
   *  UDP Server
   * *************************************************************************/
  // Create socket.
  int listenfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (listenfd <= 0){
    printf("\n ERROR: Failed to create socket. \n");
    exit(1);
  }

  // Create address.
  struct sockaddr_in address;
  memset(&address, '\0', sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(udpPort); 
  address.sin_addr.s_addr = inet_addr(serverAddr);
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  //int len_addr = sizeof()

  int bind_stat;
  bind_stat = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
  if (bind_stat == -1) {
    printf("\n ERROR: bind failed. \n");
    exit(1);
  } 

  while (1) {
    printf("\n Accepting data on UDP port.\n");
    char recvBuff[100];
    memset(recvBuff, '0', sizeof(recvBuff)); 

    unsigned int maxPackSize = sizeof(recvBuff);
    struct sockaddr_in from;
    socklen_t fromLength = sizeof(from);

    printf("\n Calling recvfrom.\n");
    int receivedBytes = recvfrom(
        listenfd, 
        (char*) recvBuff, 
        maxPackSize, 
        0, 
        (struct sockaddr*) &from, 
        &fromLength);

    if (receivedBytes <= 0) break;

    unsigned int* a, *b;
    a = (unsigned int*) malloc(1);
    b = (unsigned int*) malloc(2);

    unpackData(recvBuff, a, b);
    printf("\n Received something %d and %d.\n", *a, *b);
    int gcd = getGCD(*a, *b);
    printf("\n GCD is %d \n", gcd);

    free (a);
    free (b);
  }
   
}
  
