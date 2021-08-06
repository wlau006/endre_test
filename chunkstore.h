#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

class chunkstore{
    private:
	    std::list<std::string> values;
	    std::unordered_map<std::string, std::list<std::string>::iterator> ref;
	    int size;
	public:
	    chunkstore(int);
	    std::string insert(std::string, std::string);
};
