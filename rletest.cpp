#include "rle.h"
#include <iostream>

int main(){
    //std::string hello = "00001111";
    std::string hello = "how many shrimp do you have to eat, before you make your skin turn pink, eat too much and you'll get sick, shrimp are pretty rich";
    rle thing;
    std::cout << "ORIGINAL: " << hello << std::endl;
    std::string output = thing.encode(hello);
    std::cout << "ENCODED: " << output << std::endl;
    std::string decode = thing.decode(output);
    std::cout << "DECODED: " << decode << std::endl;
    return 0;  
}