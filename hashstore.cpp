#include "hashstore.h"


hashstore::hashstore(int capacity){
    size = capacity;
}

bool hashstore::insert(std::string val){
	bool found = false;
	if(ref.find(val) == ref.end()){
		if(ref.size() == size){
			std::string x = values.back();
			values.pop_back();
            ref.erase(x); 
		}
	}
	else{
		//std::cout << "HELLO" << std::endl;
		found = true;
	    values.erase(ref.find(val)->second);
	}
	values.push_front(val);
	ref[val]=values.begin();
	return found;
}
