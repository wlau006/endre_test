#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "cache.h"

using namespace std;

int main(){
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  char recvBuff[1025];
  
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }
  connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
  while(1){
      recv(connfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
      printf("MARKER in BUFFER: ");
      for(int i = 0; recvBuff[i] != '\0'; i++){
        printf("%c",recvBuff[i]);
      }
      printf("\n");
      strncpy(sendBuff,recvBuff, 1024);
      send(connfd, sendBuff, sizeof(recvBuff), 0);

      recv(connfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
      printf("FINGERPRINT in BUFFER: ");
      for(int i = 0; recvBuff[i] != '\0'; i++){
        printf("%c",recvBuff[i]);
      }
      printf("\n");
      strncpy(sendBuff,recvBuff, 1024);
      send(connfd, sendBuff, sizeof(recvBuff), 0);
  }
  return 0;
}
