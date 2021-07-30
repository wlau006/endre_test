#include "cache.h"


lrucache::lrucache(int capacity){
    size = capacity;
}

void lrucache::insert(unsigned int val){
	if(ref.find(val) == ref.end()){
		if(ref.size() == size){
			int x = values.back();
			values.pop_back();
            ref.erase(x); 
		}
	}
	else{
	    values.erase(ref.find(val)->second);
	}
	values.push_front(val);
	ref[val]=values.begin();
}

void lrucache::contents(){
	for(auto it=values.begin();it!=values.end();it++)
	std::cout<< *it <<" ";
	std::cout << std::endl;
}