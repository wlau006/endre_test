#pragma once

#include <string>
#include <sstream>
#include <stdio.h>

class rle{
	public:
	    std::string encode(std::string);
	    std::string decode(std::string);
		std::string encodev2(std::string);
		std::string decodev2(std::string);
};
