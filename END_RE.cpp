//Let w = 32; p = 32; Assume len ≥ w;
//where w is the minimum size of redundant bytes we would like to check
//p is the sampling rate ratio
//SAMPLETABLE[i] maps byte i to either 0 or 1
//also, p = 256/x. where x is the random locations in SAMPLETABLE set to 1.
//Jenkinshash() computes hash over a w byte window
//SAMPLEBYTE(data,len)
//for(i = 0;i < len − w;i + +)
//if (SAMPLETABLE[data[i]] == 1)
//marker = i;
//fingerprint = JenkinsHash(data + i);
//store marker,fingerprint in table;
//i = i + p/2;


uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <random>
#include <string>
#include <unordered_map>


#define FILESIZE 10000
using namespace std;

unordered_map <int,uint32_t> samplebyte (string data){
    int w = 32, p = 32, tablesize = 256, marker = 0;
    unordered_map<int,uint32_t> out;
    string temp;
    uint32_t fingerprint;

    int sampletable[tablesize] = {0};
    int x = tablesize/p;
    for(int i = 0; i < x; i++){
        sampletable[i] = 1;
    }
    unsigned seed = time(0);
    shuffle (sampletable, sampletable+tablesize, std::default_random_engine(seed));
    int filesize = FILESIZE;
    for(int i = 0; i < filesize - w; i++ ){
        unsigned char d = data.at(i);
        if (sampletable[int(d)] == 1){
            marker = i;
            for(int j = 0; j < w; j++){
                temp += data.at(j);
            }
            const uint8_t* pointer = reinterpret_cast<const uint8_t*>(temp.c_str());
            size_t jenkins = w;
            fingerprint = jenkins_one_at_a_time_hash(pointer,jenkins);
            //store marker,fingerprint in table;
            out.insert(make_pair(marker,fingerprint));
            i = i + p/2;
        }
    }
    return out;
}

int main(){
    string data;
    unordered_map <int,uint32_t> output;
    fstream f("newtext.txt", fstream::in);
    ostringstream sstr;
    sstr << f.rdbuf();
    data = sstr.str();
    
    output = samplebyte(data);

    return 0;
}