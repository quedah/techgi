

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

int main(int argc, char *argv[])
{
  printf("\n TCP/UDP server \n\n");
  
  // Check argument count.
  if (argc != 4) {
      fprintf(stderr,"Usage: server tcpPort udpPort \n");
      exit(1);
  }

  unsigned int tcpPort, udpPort;
  tcpPort = atoi(argv[1]);
  udpPort = atoi(argv[2]);

  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 

  char sendBuff[1025];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
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
}
