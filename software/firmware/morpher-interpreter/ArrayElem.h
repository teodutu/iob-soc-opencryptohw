#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cassert>

struct ArrayElem
{
    int idx, initial, final;
    std::string name;
};

std::unordered_map<std::string, std::vector<ArrayElem>> ReadArrays(
    const char* arrays_filename);
