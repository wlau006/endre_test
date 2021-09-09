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
#define CHUNKSTORESIZE 100000
//#define RLE_ENABLED 1

using namespace std;

int main(){
  int flag;
  //cin >> flag;
  rle decoder;
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[10];
  char recvBuff[10000];
  
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
  printf("Accepted connection\n");
  chunkstore CS(CHUNKSTORESIZE);
  string receivemsg1 = "";
  string receivemsg2 = "";
  string debug = "";
  string lenmsg = "";
  unsigned int numlen = 0;
  int position;
  recv(connfd,recvBuff, sizeof(recvBuff),0);
  flag = recvBuff[0] - '0';
  int counter = 0;
  while(1){
      
      position = 1;
      numlen = 0;
      lenmsg = "";
      receivemsg1 = "";
      receivemsg2 = "";
      //counter++;
      //if (counter == 100000){
      //  printf("1\n");
      //}else if(counter == 200000){
      //  counter = 0;
      //  printf("0\n");
      //}
      recv(connfd, recvBuff, sizeof(recvBuff),0);
      sprintf(sendBuff,"%c",'1');
      send(connfd, sendBuff,sizeof(sendBuff),0);
      
      if(recvBuff[0] == 'h'){

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

	    }else if(recvBuff[0] == 'c'){
		    
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

        if(flag == 1){ //uncompressing RLE

          receivemsg1 = decoder.decodev2(receivemsg1);

        }else if(flag == 2){ //uncompressing Zstd

          char* tempstr = (char *) malloc(2048);
          char* tempstr2 = (char *) malloc(receivemsg1.size());

          receivemsg1.copy(tempstr2,receivemsg1.size(),0);
          size_t tempstrlength = ZSTD_decompress(tempstr,2048,tempstr2,receivemsg1.size());
          receivemsg1 = string(&tempstr[0],tempstrlength);

          //printf("size of message: %ld\n",receivemsg1.size());

          free(tempstr);
          free(tempstr2);

        }else if (flag == 3){ //Uncompressing ZLIB

        char* tempstr2 = (char *) malloc(receivemsg1.size());
        receivemsg1.copy(tempstr2,receivemsg1.size(),0);
        Bytef* tempstr2b = (Bytef*)tempstr2;

        unsigned long templen = 2048;
        char* tempstr = (char *) malloc(templen);
        Bytef* tempstrb = (Bytef*)tempstr;

        uncompress(tempstrb,&templen, tempstr2b, receivemsg1.size());
        receivemsg1 = string(&tempstr2[0],templen);

        //printf("size of message: %ld\n",receivemsg1.size());

        free(tempstr);
        free(tempstr2);

        }

		    debug = CS.insert(receivemsg2,receivemsg1); //insert new chunk/hash into the chunkstore

	    }else if(recvBuff[0] == 'd'){ //close the connection
        break;
      }
  }
  close(connfd);
  return 0;
}
