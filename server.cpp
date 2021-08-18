#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <zstd.h>
#include <zlib.h>

#include "chunkstore.h"
#include "rle.h"

#define CHUNKSIZE 1024
#define CHUNKSTORESIZE 1000
//#define RLE_ENABLED 1

using namespace std;

int main(){
  int flag;
  cin >> flag;
  rle decoder;
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[10];
  char recvBuff[100000];
  
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
  string lenmsg = "";
  unsigned int numlen = 0;
  int position;
  while(1){
      position = 1;
      numlen = 0;
      lenmsg = "";
      receivemsg1 = "";
      receivemsg2 = "";
      recv(connfd, recvBuff, sizeof(recvBuff),0);
      //cout << recvBuff << endl;
      sprintf(sendBuff,"%c",'1');
      send(connfd, sendBuff,sizeof(sendBuff),0);
      if(recvBuff[0] == 'h'){
        //cout << "A" << endl;
        while(recvBuff[position] != '.'){
          lenmsg += recvBuff[position];
          position++;
        }
        numlen = stoi(lenmsg);
        //printf("Length of msg: %d\n",numlen);
        receivemsg1 = string(&recvBuff[position + 1],numlen);
		    debug = CS.insert(receivemsg1,"");
		    //printf("Received duplicate request, pulling chunk from cache with associated key:\n");
        //printf("%s\n",receivemsg1.c_str());
		    //printf("=========================================================\n");
		    //printf("%s\n",debug.c_str());
		    //printf("=========================================================\n");
	    }else if(recvBuff[0] == 'c'){
        //cout << "B" << endl;
		    
        while(recvBuff[position] != '.'){
          lenmsg += recvBuff[position];
          position++;
        }
        numlen = stoi(lenmsg);
        //printf("Length of msg: %d\n",numlen);
        receivemsg1 = string(&recvBuff[position + 1],numlen);
		    
        recv(connfd, recvBuff, sizeof(recvBuff),0);
		    
        position = 1;
        numlen = 0;
        lenmsg = "";
        
        while(recvBuff[position] != '.'){
          lenmsg += recvBuff[position];
          position++;
        }
        numlen = stoi(lenmsg);
        //printf("Length of msg: %d\n",numlen);
		    receivemsg2 = string(&recvBuff[position + 1],numlen);
        sprintf(sendBuff,"%c",'1');
        send(connfd, sendBuff,sizeof(sendBuff),0);
		    //printf("Received new chunk, inserting into cache\n");
        if(flag == 1){
          receivemsg1 = decoder.decode(receivemsg1);
          //cout << "Hello" << endl;
        }else if(flag == 2){
          char* tempstr = (char *) malloc(2048);
          char* tempstr2 = (char *) malloc(receivemsg1.size());
          receivemsg1.copy(tempstr2,receivemsg1.size(),0);
          size_t tempstrlength = ZSTD_decompress(tempstr,2048,tempstr2,receivemsg1.size());
          receivemsg1 = string(&tempstr[0],tempstrlength);
          free(tempstr);
          free(tempstr2);
        }
		    debug = CS.insert(receivemsg2,receivemsg1);
	    }else if(recvBuff[0] == 'd'){
        break;
      }
  }
  close(connfd);
  return 0;
}
