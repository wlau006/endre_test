#include "chunkstore.h"


chunkstore::chunkstore(int capacity){
    size = capacity;
}

std::string chunkstore::insert(std::string val, std::string data){
	std::string output = "";
	//std::cout << val << std::endl;
	//std::cout << data << std::endl;
	if(ref.find(val) == ref.end()){
		//printf("1\n");
		if(ref.size() == size){
			std::string x = values.back();
			values.pop_back();
            ref.erase(x);
		}
		values.push_front(data);
	}else{
		//printf("2\n");
		auto it = ref.find(val)->second;
		//std::cout << *it << std::endl;
		output = *it;
	    values.erase(it);
	    values.push_front(output);
	}
	ref[val]=values.begin();
	return output;
}
