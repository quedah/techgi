
/*
 *  TechGI 4
 *  Praktische Aufgabe #1
 *
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define MAX_BUFFER_LENGTH 100

int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
  memset(buffer, '\0', sizeof(buffer));
  memcpy(&buffer[1], (unsigned char*)&a, sizeof(unsigned int));
  memcpy(&buffer[3], (unsigned char*)&b, sizeof(unsigned int));
}

void unpackData(unsigned char* buffer, unsigned int* a, unsigned int* b) {
  *a = (buffer[0]<<8)|buffer[1];
  *b = (buffer[2]<<8)|buffer[3];
  printf("\n Unpacked values are:   %d  %d \n", *a, *b);
}

int main(int argc, char *argv[]) {
  int sockfd = 0, n = 0;
  int serverPort;
  struct hostent *he;
  char recvBuff[1024];
  struct sockaddr_in serv_addr;     // in <netinet/in.h>
  uint32_t a, b;

  printf("TCP client example\n\n");
  
  // Check argument count.
  if (argc != 5) {
      fprintf(stderr,"Usage: tcpClient serverName serverPort int1 int2\n");
      exit(1);
  }

  serverPort = atoi(argv[2]);
  a = atoi(argv[3]);
  b = atoi(argv[4]);    
  
  //Resolv hostname to IP Address
  if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
      herror("gethostbyname");
      exit(1);
  } else {
    printf("\n Get host info OK\n");
  }
  // Fill buffer memory allocation with something.
  memset(recvBuff, '0', sizeof(recvBuff));
  
  // sockfd - Get socket descriptor. Returns -1 if failed.
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  } else {
    printf("\n Create socket OK\n");
  }

  // Fill server address mem. allocation with s.th.
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(serverPort);

  // inet_pton - convert IPv4 addr. from text to binary form
  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    printf("\n inet_pton error occured\n");
    return 1;
  } else {
    printf("\n Inet Pton OK\n");
  }

  unsigned char* sendBuff;
  sendBuff = (unsigned char*) malloc(4);
  packData(sendBuff, a, b);

  if (sendto( sockfd, 
              sendBuff, 
              sizeof(sendBuff), 
              0,
              (struct sockaddr*) 
              &serv_addr, 
              sizeof(serv_addr)) < 0) {
    printf("\n Error : Send Failed \n");
    return 1;
  } else {
    printf("\n Send OK\n");
  }

  unsigned int* d;
  unsigned int* e;
  d = (unsigned int*) malloc(1);
  e = (unsigned int*) malloc(1);
  printf("\n Entered Data:  %d    %d \n" , a, b);
  printf("\n sendBuff content at 0  %u \n", sendBuff[0]);
  printf("\n sendBuff content at 1  %u \n", sendBuff[1]);
  printf("\n sendBuff content at 2  %u \n", sendBuff[2]);
  printf("\n sendBuff content at 3  %u \n", sendBuff[3]);
  unpackData(sendBuff, d, e);

  return 0;
}
