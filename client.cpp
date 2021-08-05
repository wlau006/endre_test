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
#include "END_RE.h"
#include "hashstore.h"
#include <iostream>
#include <fstream>
#include "sha1.hpp"

#define BLOCKSIZE 4096 //this will be a divisor of the filesize in bytes in our case.
#define CHUNKSIZE BLOCKSIZE/4
#define HASHSTORESIZE 100

using namespace std;



int main(){
  int sockfd = 0,n = 0;
  char recvBuff[1024];
  char sendBuff[2048];
  struct sockaddr_in serv_addr;

  //printf("1\n");
  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  if(inet_pton(AF_INET, "10.0.0.1", &serv_addr.sin_addr)<=0){
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }
  //printf("2\n");
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);



  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
    printf("\n Error : Connect Failed \n");
    return 1;
  }
  ostringstream file1;
  fstream f1("oldtext.txt", fstream::in);
  hashstore HS(HASHSTORESIZE);
  SHA1 hashing_func;
  char * charbuffer = new char [CHUNKSIZE+1];
  int i = 0;
  while(!f1.eof()){
	string hash = "h";
	string chunk = "c";
	f1.seekg(CHUNKSIZE * i, f1.beg);
	f1.read(charbuffer,CHUNKSIZE);
	
	charbuffer[CHUNKSIZE] = '\0';
	string funcinput = charbuffer;
	hashing_func.update(funcinput);
	string hashout = hashing_func.final();
	
	if(HS.insert(hashout)){
		printf("Found duplicate chunk, sending hash to receiver\n");
		hash += hashout;
		sprintf(sendBuff,"%s",hash.c_str());
		send(sockfd,sendBuff,sizeof(sendBuff),0);
	}else{
		printf("New chunk, sending chunk, then hash to receiver\n");
		chunk += funcinput;
		sprintf(sendBuff,"%s",chunk.c_str());
		send(sockfd,sendBuff,sizeof(sendBuff),0);
		sprintf(sendBuff,"%s",hashout.c_str());
		send(sockfd,sendBuff,sizeof(sendBuff),0);
	}
	i++;
  }

  return 0;
}
