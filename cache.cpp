#include "cache.h"


lrucache::lrucache(int capacity){
    size = capacity;
}

void lrucache::insert(std::string val){
	if(ref.find(val) == ref.end()){
		if(ref.size() == size){
			std::string x = values.back();
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