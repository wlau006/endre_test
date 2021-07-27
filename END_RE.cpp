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
#include <stdint.h>


#define FILESIZE 10000
using namespace std;

vector<int> sampletable (int tablesize, int p){
    vector<int> sampletable(tablesize, 0);
    int x = tablesize/p;
    cout << "X: " << x << endl;
    for(int i = 0; i < x; i++){
        sampletable.at(i) = 1;
    }
    unsigned seed = time(0);
    shuffle (sampletable.begin(), sampletable.end(), std::default_random_engine(seed));
    for(auto i = 0; i < sampletable.size(); i++){
        cout << sampletable.at(i);
    }
    cout << endl;
    return sampletable;
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length){
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    //cout << hash << endl;
    hash += hash << 10;
    //cout << hash << endl;
    hash ^= hash >> 6;
    //cout << hash << endl;
  }
  hash += hash << 3;
  //cout << hash << endl;
  hash ^= hash >> 11;
  //cout << hash << endl;
  hash += hash << 15;
  //cout << hash << endl;
  //cout << "______________" << endl;
  return hash;
}

unordered_map <int,uint32_t> samplebyte (string data, int p, vector<int> sampletable){
    int w = 32, marker = 0;
    unordered_map<int,uint32_t> out;
    string temp = "";
    uint32_t fingerprint;
    int debughitcounter = 0;
    int filesize = FILESIZE;
    cout << "Filesize: " << FILESIZE << endl;
    for(int i = 0; i < filesize - w; i++ ){
        unsigned char d = data.at(i);
        //cout << d << endl;
        if (sampletable.at(int(d)) == 1){
            debughitcounter++;
            marker = i;
            for(int j = i; j < w; j++){
                temp += data.at(j);
            }
            size_t jenkins = w;
            uint8_t* pointer = reinterpret_cast <uint8_t*>(&temp[0]);
            fingerprint = jenkins_one_at_a_time_hash(pointer,jenkins);
            //cout << fingerprint << endl;
            //store marker,fingerprint in table;
            out.insert(make_pair(marker,fingerprint));
            i = i + p/2;
        }
        temp = "";
    }
    cout << "SampleTable Hits: " << debughitcounter << endl;
    return out;
}


int main(){
    string data;
    int tablesize = 256, p = 32;
    vector<int> st = sampletable(tablesize,p);

    unordered_map <int,uint32_t> oldfingerprint;
    fstream f("oldtext.txt", fstream::in);
    ostringstream sstr;
    sstr << f.rdbuf();
    data = sstr.str();
    cout << data << endl;

    oldfingerprint = samplebyte(data,p,st);

    for(auto it = oldfingerprint.cbegin(); it != oldfingerprint.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }

    return 0;
}