#include "rle.h"

using namespace std;


void showbits( unsigned char x )
{
    int i=0;
    for (i = (sizeof(char) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1u << i) ? '1' : '0');
    }
    printf("\n");
}

string uleb128(string rleoutput, int runlength){
  char byte;
  do {
    byte = runlength & 0x7F;
    //printf("%d,",runlength);
    //showbits(byte);
    runlength >>= 7;
    if (runlength != 0){ /* more bytes to come */
      byte = byte | 0x80;
    }
    //showbits(byte);
    rleoutput += byte;
  } while (runlength != 0);
  return rleoutput;
}

string rle::encode(string input){
    unsigned char current;
    unsigned char mask = 0x01;
    int runlength = 0;
    string rleoutput = "";
    bool which;
    //showbits(input.at(0)>>7 & 0x01);
    if(((input.at(0) >> 7) & 0x01) == 0x01){
        which = true;
        rleoutput += "1,";
    }else{
        which = false;
        rleoutput += "0,";
    }

    for(int i = 0; i < input.size(); i++){
      current = input.at(i);
      //printf("character: %c\n",current);
      //showbits(current);
      for(int j = 7; j >= 0; --j){
        //showbits(current >> j & mask);
        if(current >> j & mask == 0x01){
          if(which){
            runlength++;
          }else{
            rleoutput += to_string(runlength) + ',';
            runlength = 0;
            runlength++;
            which = true;
          }
        }else{
          if(which){
            rleoutput += to_string(runlength) + ',';
            runlength = 0;
            runlength++;
            which = false;
          }else{
            runlength++;
          }
        }
      }
    }
    rleoutput += to_string(runlength);
    //printf("Encoded output: %s\n",rleoutput.c_str());
    //printf("length of rleoutput: %li\n", rleoutput.size());
    return rleoutput;
}

string rle::decode(string input){
    string rleoutput = "";
    stringstream ss(input);
    string current;
    char mask = 0x01;
    char temp = 0x00;
    int position = 0;
    bool which;
    //printf("%s\n",input.c_str());
    getline(ss,current,',');
    if(current == "1"){
        which = true;
    }else if(current == "0"){
        which = false;
    }  

    while(getline(ss,current,',')){
        if(which){
            for(int i = 0; i < stoi(current); i++){
                temp = (temp << 1) | mask;
                //showbits(temp);
                position++;
                if(position == 8){
                    rleoutput += temp;
                    position = 0;
                    temp = 0x00;
                }
            }
            which = false;
        }else{
            for(int i = 0; i < stoi(current); i++){
                temp = temp << 1;
                position++;
                if(position == 8){
                    rleoutput += temp;
                    position = 0;
                    temp = 0x00;
                }
            }
            which = true;
        }
    }
    return rleoutput;
}


string rle::encodev2(string input){
    unsigned char current;
    unsigned char mask = 0x01;
    int runlength = 0;
    string rleoutput = "";
    bool which;

    if((input.at(0) >> 7 & 0x01) == 0x01){
        which = true;
        rleoutput += "1";
    }else{
        which = false;
        rleoutput += "0";
    }

    for(int i = 0; i < input.size(); i++){
      current = input.at(i);
      //printf("character: %c\n",current);
      //showbits(current);
      for(int j = 7; j >= 0; --j){
        //showbits(current >> j & mask);
        if(current >> j & mask == 0x01){
          if(which){
            runlength++;
          }else{
            rleoutput = uleb128(rleoutput,runlength);
            runlength = 0;
            runlength++;
            which = true;
          }
        }else{
          if(which){
            rleoutput = uleb128(rleoutput,runlength);
            runlength = 0;
            runlength++;
            which = false;
          }else{
            runlength++;
          }
        }
      }
    }

    rleoutput = uleb128(rleoutput,runlength);
    //printf("Encoded output: %s\n",rleoutput.c_str());
    //printf("length of rleoutput: %li\n", rleoutput.size());
    //for(int i = 0; i < rleoutput.size(); i++){
    //  showbits(rleoutput.at(i));
    //}
    return rleoutput;
}

string rle::decodev2(string input){
    string rleoutput = "";
    string current;
    bool which;
    char byte;
    char mask = 0x01;
    char temp = 0x00;
    int position = 0;
    int result = 0;
    int shift = 0;

    //printf("%s\n",input.c_str());
    current = input.at(0);
    if(current == "1"){
        which = true;
    }else if(current == "0"){
        which = false;
    }
    input.erase(input.begin());

    for(int i = 0; i < input.size(); i++){
      result = 0;
      shift = 0;
      while (i < input.size()) {
        byte = input.at(i);
        //showbits(byte);
        result |= byte & 0x7F << shift;
        if(((byte >> 7) & 0x01) == 0x00){
          //printf("%d\n",result);
          break;
        }
        shift += 7;
        i++;
      }

      if(which){
        for(int i = 0; i < result; i++){
            temp = (temp << 1) | mask;
            position++;
            if(position == 8){
                rleoutput += temp;
                position = 0;
                temp = 0x00;
            }
        }
        which = false;
      }else{
        for(int i = 0; i < result; i++){
            temp = temp << 1;
            position++;
            if(position == 8){
                rleoutput += temp;
                position = 0;
                temp = 0x00;
            }
        }
        which = true;
      }
    }
    return rleoutput;
}