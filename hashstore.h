#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

class hashstore{
    private:
	    std::list<std::string> values;
	    std::unordered_map<std::string, std::list<std::string>::iterator> ref;
	    int size;
	public:
	    hashstore(int);
	    bool insert(std::string);
};
