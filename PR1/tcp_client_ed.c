
/*
 *  TechGI 4
 *  Praktische Aufgabe #1
 *
 */

#include <iostream>
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

int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
  memset(buffer, '\0', sizeof buffer);
  memcpy(&buffer[1], (unsigned char*)&a, sizeof(unsigned int));
  memcpy(&buffer[3], (unsigned char*)&b, sizeof(unsigned int));
}

int main(int argc, char *argv[]) {
  int sockfd = 0, n = 0;
  char recvBuff[1024];
  // Structs defined in <netinet/in.h>
  struct sockaddr_in serv_addr;
  int a, b;

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
  }
  // Fill buffer memory allocation with something.
  memset(recvBuff, '0', sizeof(recvBuff));
  
  // sockfd - Get socket descriptor. Returns -1 if failed.
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  // Fill server address mem. allocation with s.th.
  memset(&serv_addr, '0', sizeof(serv_addr));

  // Specify protocol family (=> IPv4)
  serv_addr.sin_family = AF_INET;
  // Specify port.
  serv_addr.sin_port = htons(5000);

  // inet_pton - convert IPv4 addr. from text to binary form
  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    printf("\n inet_pton error occured\n");
    return 1;
  }

  unsigned char* sendBuff;
  packData(sendBuff, a, b);
  write(sockfd, sendBuff, sizeof(sendBuff));


  // connect - bind socket to local address. Returns 0 if success, -1 else.
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("\n Error : Connect Failed \n");
    return 1;
  }

  write(sockfd, sendBuff, sizeoof(sendBuf));
  // read - Read data from socket with descriptor 'sockfd' and store it in
  //        buffer 'recvBuff'
  while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
    recvBuff[n] = 0;
    // fputs - write recvBuff to stdout stream.
    // Returns non-neg. value on success, EOF on fail.
    if (fputs(recvBuff, stdout) == EOF) {
      printf("\n Error : Fputs error\n");
    }
  }

  if (n < 0) {
    printf("\n Read error \n");
  }

  return 0;
}
