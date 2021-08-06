#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "chunkstore.h"

#define CHUNKSIZE 1024
#define CHUNKSTORESIZE 1000

using namespace std;

int main(){
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[1024];
  char recvBuff[2048];
  
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
  
  chunkstore CS(CHUNKSTORESIZE);
  string receivemsg1 = "";
  string receivemsg2 = "";
  string debug = "";
  while(1){
      recv(connfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
      receivemsg1 = recvBuff;
      if(receivemsg1[0] == 'h'){
        //cout << "A" << endl;
		    receivemsg1.erase(receivemsg1.begin());
		    debug = CS.insert(receivemsg1,"");
		    printf("Received duplicate request, pulling chunk from cache with associated key:\n");
        printf("%s\n",receivemsg1.c_str());
		    //printf("=========================================================\n");
		    //printf("%s\n",debug.c_str());
		    //printf("=========================================================\n");
	    }else if(receivemsg1[0] == 'c'){
        //cout << "B" << endl;
		    receivemsg1.erase(receivemsg1.begin());
		    recv(connfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
		    receivemsg2 = recvBuff;
		    printf("Received new chunk, inserting into cache\n");
		    debug = CS.insert(receivemsg2,receivemsg1);
	    }else if(receivemsg1[0] == 'd'){
        break;
      }
  }
  close(connfd);
  return 0;
}
