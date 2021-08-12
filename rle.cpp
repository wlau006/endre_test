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

string rle::encode(string input){
    unsigned char current;
    unsigned char mask = 0x01;
    int runlength = 0;
    string rleoutput = "";
    bool which;

    if((input.at(0) >> 7) == 0x01){
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
            //  printf("a\n");
            runlength++;
          }else{
            //  printf("b\n");
            rleoutput += to_string(runlength) + ',';
            runlength = 0;
            runlength++;
            which = true;
          }
        }else{
          if(which){
            //  printf("c\n");
            rleoutput += to_string(runlength) + ',';
            runlength = 0;
            runlength++;
            which = false;
          }else{
            //  printf("d\n");
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
