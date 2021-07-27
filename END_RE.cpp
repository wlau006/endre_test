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
    hash += hash << 10;
    hash ^= hash >> 6;
    
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

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

unordered_map <int,uint32_t> samplebyte (string data, int p, vector<int> sampletable, int w){
    int marker = 0;
    unordered_map<int,uint32_t> out;
    string temp = "";
    uint32_t fingerprint;
    int debughitcounter = 0;
    int filesize = FILESIZE;
    for(int i = 0; i < filesize - w; i++ ){
        unsigned char d = data.at(i);
        //cout << d << endl;
        if (sampletable.at(int(d)) == 1){
            debughitcounter++;
            marker = i;
            for(int j = 0; j < w; j++){
                temp += data.at(i+j);
            }
            //cout << temp << endl;
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


//Let w = 32; p = 32; Assume len ≥ w;
//RabinHash() computes RABIN hash over a w byte window
//MODP(data,len)
//for(i = 0;i < w − 1; i + +)
//fingerprint = RabinHash(data[i]);
//for(i = w − 1;i < len;i + +)
//fingerprint = RabinHash(data[i]);
//if (fingerprint % p == 0) //MOD
//marker = i − w + 1;
//store marker,fingerprint in table;



unordered_map <int,uint32_t> modp (string data, int p, int w){
    #define d 256

    //q is a prime randomly selected
    int q = 11359393;
    int fingerprint = 0; // hash value for data
    int h = 1;
    int length = FILESIZE;
    int marker = 0;
    unordered_map<int,uint32_t> out;
 
    // The value of h would be "pow(d, w-1)%q"
    for (int i = 0; i < w - 1; i++){
        h = (h * d) % q;
    }
 
    // Calculate the hash value of pattern and first
    // window of text
    for (int i = 0; i < w - 1; i++)
    {
        fingerprint = (d * fingerprint + data.at(i)) % q;
    }
 
    // Slide the pattern over text one by one
    for (int i = w - 1; i < length; i++)
    {
        // Calculate hash value for next window of text: Rewove
        // leading digit, add trailing digit
        fingerprint = (d*(fingerprint - data.at(i)*h) + data.at(i+w))%q;

        if (fingerprint < 0){
            fingerprint = (fingerprint + q);
        }

        if (fingerprint % p == 0)
        {
            marker = i - w + 1;
            out.insert(make_pair(marker,fingerprint));
            //cout << fingerprint << endl;
        }
    }
    return out;
}

unordered_map <int,uint32_t> maxp (string data, int p, int w){
    int marker = 0;
    int max = 0;
    unordered_map<int,uint32_t> out;
    string temp = "";
    uint32_t fingerprint;
    int length = FILESIZE;
    for (int i = 0; i < length; i = i + p){
        for(int j = 0; j < p; j++){
            if (max < data.at(j + i)){
                max = data.at(j + i);
                marker = j+i;
            }
        }
        for(int j = 0; j < w; j++){
                temp += data.at(marker+j);
        }
        size_t jenkins = w;
        uint8_t* pointer = reinterpret_cast <uint8_t*>(&temp[0]);
        fingerprint = jenkins_one_at_a_time_hash(pointer,jenkins);
        out.insert(make_pair(marker,fingerprint));
        temp = "";
        max = 0;
    }
    return out;
}

unordered_map <int,uint32_t> fixed (string data, int p, int w){
    int marker = 0;
    unordered_map<int,uint32_t> out;
    string temp = "";
    uint32_t fingerprint;
    int length = FILESIZE;
    for (int i = p; i < length; i = i + p){
        marker = i;
        for(int j = 0; j < w; j++){
                temp += data.at(i+j);
        }
        size_t jenkins = w;
        uint8_t* pointer = reinterpret_cast <uint8_t*>(&temp[0]);
        fingerprint = jenkins_one_at_a_time_hash(pointer,jenkins);
        out.insert(make_pair(marker,fingerprint));
        temp = "";
    }
    return out;
}


int main(){
    string data, data2;
    int tablesize = 256, w = 32, p = 32;
    vector<int> st;

    cout << "Filesize: " << FILESIZE << endl;

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

    for(auto it = oldfingerprint.cbegin(); it != oldfingerprint.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
    
    cout << "---------------------------------------------------" << endl;

    for(auto it = newfingerprint.cbegin(); it != newfingerprint.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
    return 0;
}