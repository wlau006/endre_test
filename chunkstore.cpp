#include "chunkstore.h"


chunkstore::chunkstore(int capacity){
    size = capacity;
}

std::string chunkstore::insert(std::string val, std::string data){
	std::string output = "";
	if(data != ""){
		if(ref.size() == size){
			std::string x = values.back().first;
			values.pop_back();
            ref.erase(x);
		}
		values.push_front(make_pair(val,data));
	}else{
		auto it = ref.find(val)->second;
		output = it->second;
	    values.erase(it);
	    values.push_front(make_pair(val,output));
	}
	ref[val]=values.begin();
	return output;
}
