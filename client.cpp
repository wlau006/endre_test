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
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <zstd.h>
#include <zlib.h>

#include "hashstore.h"
#include "sha1.hpp"
#include "rle.h"

#define CHUNKSIZE 1024 //this will be a divisor of the filesize in bytes in our case.
#define HASHSTORESIZE 10000 //SIZE IN MAX ENTRIES
//#define RLE_ENABLED 1

using namespace std;



int main(){

  rle encoder;
  int flag;
  cin >> flag; //flag to determine which scheme we are using ontop of chunkmatch

  int sockfd = 0,n = 0;

  char recvBuff[10];
  char sendBuff[100000]; //size of send buffer, dont think this really needs to be this big

  struct sockaddr_in serv_addr;
  int cachehits = 0; //how many times we got a cache hit.

  // SOCKET CONNECTION SETUP
  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  if(inet_pton(AF_INET, "10.0.0.1", &serv_addr.sin_addr)<=0){
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);



  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
    printf("\n Error : Connect Failed \n");
    return 1;
  }
  sleep(1);
  
  auto start = chrono::high_resolution_clock::now(); //TIMER START

  ostringstream file1;
  fstream f1("input.txt", fstream::in); //OPEN INPUT FILE
  f1.seekg (0, f1.end);
  int filelength = f1.tellg(); //GET FILE LENGTH
  int bytes_sent = 0;

  hashstore HS(HASHSTORESIZE); //SET CACHE SIZE
  SHA1 hashing_func;
  char * charbuffer = new char [CHUNKSIZE];
  int i = 0;
  while(1){
	  string hash = "h";
	  string chunk = "c";

    f1.seekg(CHUNKSIZE * i, f1.beg);
	  f1.read(charbuffer,CHUNKSIZE);
    if(f1.eof()){
      break;
    }

    string funcinput = string(&charbuffer[0],1024);
	  hashing_func.update(funcinput);
	  string hashout = hashing_func.final();
	  //cout << "Calculated hash: " << hashout << endl; //DEBUG

    //This section tries to insert into hashstore, if there already exists one, it will return true but not insert a new entry.
    //
    //If the cache is full, it uses LRU to replace the entry in the hash store.
    if(HS.insert(hashout)){
      
      cachehits++;
	  	
      //printf("Found duplicate chunk, sending hash to receiver\n"); //DEBUG
      
      hash += to_string(hashout.size()) + "." + hashout;
      bytes_sent += hash.size();
      
      //cout << "Size of hash: " << hash.size() << endl; //DEBUG
      //cout << "Hash msg: " << hash << endl; //DEBUG
      //cout << "Total Bytes Sent: " << bytes_sent << endl; //DEBUG
      
      hash.copy(sendBuff,hash.size(),0);
	  	send(sockfd,sendBuff,hash.size(),0); //Sends hash
      recv(sockfd,recvBuff,sizeof(recvBuff),MSG_WAITALL); //Waits for confirmation signal from server to avoid synchronization errors

	  }else{
	  	//printf("New chunk, sending chunk, then hash to receiver\n"); //DEBUG
      
      if(funcinput.size() != 0 && flag == 1){ //Encodes our chunk with RLE
      
      //  cout << "Size of input before: " << funcinput.size() << endl; //DEBUG
      
        funcinput = encoder.encodev2(funcinput);
      
      //  cout << "Size of input after RLE: " << funcinput.size() << endl; //DEBUG
      
      }else if(funcinput.size() != 0 && flag == 2){ //Encode our chunk with ZSTD compression
        
        char* tempstr = (char *) malloc(funcinput.size());
        char* tempstr2 = (char *) malloc(2048);
        
        funcinput.copy(tempstr,funcinput.size(),0);
        size_t zstdlength = ZSTD_compress(tempstr2,2048,tempstr,funcinput.size(),10);
        funcinput = string(&tempstr2[0],zstdlength);
        
        free(tempstr);
        free(tempstr2);

      }else if (funcinput.size() != 0 && flag == 3){ //Encodes our chunk with ZLIB compression
        
        Bytef* tempstr = (Bytef *) malloc(funcinput.size());
        funcinput.copy((char*)tempstr,funcinput.size(),0);
        
        unsigned long templen = 2048;
        Bytef* tempstr2 = (Bytef *) malloc(templen);
        
        compress2(tempstr2,(uLongf*)&templen, tempstr, (uLong)funcinput.size(),1);
        funcinput = string((char*)&tempstr2[0],templen);
        
        free(tempstr);
        free(tempstr2);
        
      }

      chunk += to_string(funcinput.size()) + "." + funcinput;

      //cout << "Size of message: " << chunk.size() << endl; //DEBUG

      bytes_sent += chunk.size();
      
      chunk.copy(sendBuff,chunk.size(),0);
	  	send(sockfd,sendBuff,chunk.size(),0);
      recv(sockfd,recvBuff,sizeof(recvBuff),MSG_WAITALL);
      
      hash += to_string(hashout.size()) + "." + hashout;

      //cout << "Size of hash: " << hash.size() << endl; //DEBUG
      
      bytes_sent += hash.size();
      
      //cout << "Hash msg: " << hash << endl; //DEBUG
      //cout << "Total Bytes Sent: " << bytes_sent << endl; //DEBUG
      
      hash.copy(sendBuff,hash.size(),0);
	  	send(sockfd,sendBuff, hash.size(),0);
      recv(sockfd,recvBuff,sizeof(recvBuff),MSG_WAITALL);
	  }

	  i++;

  }

  char done = 'd';
  sprintf(sendBuff,"%c",done);
  send(sockfd,sendBuff,sizeof(sendBuff),0);
  
  f1.close();

  auto stop = chrono::high_resolution_clock::now(); //Stop timer
  
  // Get duration. Substart timepoints to 
  // get durarion. To cast it to proper unit
  // use duration cast method

  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
  double percent = (double)bytes_sent/(double)filelength;
  
  cout << endl << endl;
  cout << "=====================================================================" << endl;
  if(flag == 1){
    cout << "RLE ENABLED" << endl;
  }else if(flag == 2){
    cout << "ZSTD ENABLED" << endl;
  }else if(flag == 3){
    cout << "ZLIB ENABLED" << endl;
  }
  cout << "Time taken to parse input file of size " << filelength << " bytes: " << duration.count() << " milliseconds" << endl;
  cout << "Bytes transferred between sender and receiver: " << bytes_sent << endl;
  cout << "Total number of hashstore hits: " << cachehits << endl;
  double reduction = 100 - (percent * 100);
  if(reduction < 0){
    reduction = -reduction;
    reduction + 100;
    cout << "Bytes transferred increased by: " << reduction << "%" << endl;
  }else{
    cout << "Bytes transferred reduced by: " << reduction << "%" << endl;
  }
  cout << "=====================================================================" << endl << endl;


  return 0;
}
