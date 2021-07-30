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
  

  string data, data2;
  int tablesize = 256, w = 32, p = 32;
  vector<int> st;
  cout << "Filesize: " << 10000 << endl;
  unordered_map <int,uint32_t> oldfingerprint;
  unordered_map <int,uint32_t> newfingerprint;
  fstream f1("oldtext.txt", fstream::in);
  ostringstream file1;
  file1 << f1.rdbuf();
  data = file1.str();
  //cout << data << endl;
  fstream f2("newtext.txt", fstream::in);
  ostringstream file2;
  file2 << f2.rdbuf();
  data2 = file2.str();
  int input = 0;
  cout << "0(samplebyte) or 1(modp) or 2(maxp) or 3(fixed)" << endl;
  cin >> input;
  switch(input){
    case 0:
      st = sampletable(tablesize,p);
      oldfingerprint = samplebyte(data,p,st,w);
      newfingerprint = samplebyte(data2,p,st,w);
    break;
    case 1:
      oldfingerprint = modp(data,p,w);
      newfingerprint = modp(data2,p,w);
    break;
    case 2:
      oldfingerprint = maxp(data,p,w);
      newfingerprint = maxp(data2,p,w);
    break;
    case 3:
      oldfingerprint = fixed(data,p,w);
      newfingerprint = fixed(data2,p,w);
    break;
  }
  //for(auto it = oldfingerprint.cbegin(); it != oldfingerprint.cend(); ++it)
  //{
  //    std::cout << it->first << " " << it->second << "\n";
  //}
  //
  //cout << "---------------------------------------------------" << endl;
  //for(auto it = newfingerprint.cbegin(); it != newfingerprint.cend(); ++it)
  //{
  //    std::cout << it->first << " " << it->second << "\n";
  //}
  for(auto it = oldfingerprint.cbegin(); it != oldfingerprint.cend(); ++it){
    cout << "key being sent: " << it->first << endl;
    sprintf(sendBuff, "%d", it->first);
    cout << "SEND BUFFER: ";
    for(int i = 0; sendBuff[i] != '\0'; i++){
      cout << sendBuff[i];
    }
    cout << endl;
    send(sockfd, sendBuff, sizeof(sendBuff), 0);
    recv(sockfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
    cout << "RECV BUFFER: ";
    for(int i = 0; recvBuff[i] != '\0'; i++){
      cout << recvBuff[i];
    }
    cout << endl;
    cout << "fingerprint being sent: " << it->second << endl;
    sprintf(sendBuff, "%u", it->second);
    cout << "SEND BUFFER: ";
    for(int i = 0; sendBuff[i] != '\0'; i++){
      cout << sendBuff[i];
    }
    cout << endl;
    send(sockfd, sendBuff, sizeof(sendBuff), 0);
    recv(sockfd, recvBuff, sizeof(recvBuff), MSG_WAITALL);
    cout << "RECV BUFFER: ";
    for(int i = 0; recvBuff[i] != '\0'; i++){
      cout << recvBuff[i];
    }
    cout << endl;
  }
  return 0;
}
