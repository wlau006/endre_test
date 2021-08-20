#include "rle.h"
#include <iostream>

int main(){
    //std::string hello = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x42\x42\x42\x42\x42\x42\x42";
    std::string hello = "how many shrimp do you have to eat, before you make your skin turn pink, eat too much and you'll get sick, shrimp are pretty rich";
    rle thing;
    std::cout << "V1" << std::endl;
    std::cout << "ORIGINAL: " << hello << std::endl;
    std::string output = thing.encode(hello);
    std::cout << "ENCODED: " << output << std::endl;
    std::string decode = thing.decode(output);
    std::cout << "DECODED: " << decode << std::endl;

    if(hello != decode){
        std::cout << "RLE FAILED" << std::endl;
    }else{
        std::cout << "RLE SUCCESS" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "==========================================" << std::endl << std::endl;
    

    std::cout << "V2" << std::endl;
    std::cout << "ORIGINAL: " << hello << std::endl;
    output = thing.encodev2(hello);
    std::cout << "ENCODED: " << output << std::endl;
    decode = thing.decodev2(output);
    std::cout << "DECODED: " << decode << std::endl;

    if(hello != decode){
        std::cout << "RLE FAILED " << std::endl;
    }else{
        std::cout << "RLE SUCCESS" << std::endl;
    }

    return 0;  
}