#pragma once
#include<iostream>
#include<unordered_map>
#include<list>

class lrucache{
    private:
	    std::list<unsigned int> values;
	    std::unordered_map<unsigned int, std::list<unsigned int>::iterator> ref;
	    int size;
	public:
	    lrucache(int);
	    void insert(unsigned int);
	    void contents(); //debug
};