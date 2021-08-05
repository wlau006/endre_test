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
#include "cache.h"
#include <iostream>
#include <fstream>
#include "sha1.hpp"

using namespace std;

int main(){
  int sockfd = 0,n = 0;
  char recvBuff[1025];
  char sendBuff[1025];
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
  int tablesize = 256, w = 32, p = 32;
  vector<int> st;
  unordered_map <int,uint32_t> oldfingerprint;
  unordered_map <int,uint32_t> newfingerprint;
  ostringstream file1;
  ostringstream file2;
  fstream f1("oldtext.txt", fstream::in);
  fstream f2("newtext.txt", fstream::in);
  string data, data2;
      int partition_size = 10000/4;
      for(int i = 0; i < 4; i++){
       f1.seekg(partition_size * i, f1.beg);
       char * charbuffer = new char [partition_size];
       f1.read(charbuffer,partition_size);
       string funcinput = charbuffer;
       SHA1 hashing_func;
       hashing_func.update(funcinput);
       string hashout = hashing_func.final();
       sprintf(sendBuff,"%c",hashout);
       send(sockfd,sendBuff,sizeof(sendBuff),0);
      }

  return 0;
}
