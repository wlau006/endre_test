#include "chunkstore.h"


chunkstore::chunkstore(int capacity){
    size = capacity;
}

std::string chunkstore::insert(std::string val, std::string data){
	std::string output = "";
	if(ref.find(val) == ref.end()){
		if(ref.size() == size){
			std::string x = values.back();
			values.pop_back();
            ref.erase(x);
		}
		values.push_front(data);
	}else{
		auto it = ref.find(val)->second;
		output = *it;
	    values.erase(it);
	    values.push_front(output);
	}
	ref[val]=values.begin();
	return output;
}
