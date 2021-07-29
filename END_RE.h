#pragma once
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <random>
#include <string>
#include <unordered_map>
#include <stdint.h>


std::vector<int> sampletable (int tablesize, int p);
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);
std::unordered_map <int,uint32_t> samplebyte (std::string data, int p, std::vector<int> sampletable, int w);
std::unordered_map <int,uint32_t> modp (std::string data, int p, int w);
std::unordered_map <int,uint32_t> maxp (std::string data, int p, int w);
std::unordered_map <int,uint32_t> fixed (std::string data, int p, int w);
