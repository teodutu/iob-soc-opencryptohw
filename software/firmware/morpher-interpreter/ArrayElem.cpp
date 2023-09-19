#include "ArrayElem.h"

static int ReadNextToken(std::stringstream& s, std::string token)
{
    getline(s, token, ',');
    return std::stoi(token);    
}

static std::istream& operator>>(std::istream& is, ArrayElem& e)
{
    const size_t size = sizeof(e.initial);
    std::string line, token;
    int init[4], fin[4];

    for (size_t i = 0; i < size; i++) {
        getline(is, line);
        std::stringstream s(line);

        getline(s, e.name, ',');
        if (e.name[0] == '\n')
            e.name.erase(0, 1);

        if (e.name.empty())
            break;

        e.idx = ReadNextToken(s, token);
        init[i] = ReadNextToken(s, token);
        fin[i] = ReadNextToken(s, token);

        if (e.name == "loopstart" || e.name == "loopend")
            break;
    }

    e.initial = init[0] | (init[1] << 8) | (init[2] << 16) | (init[3] << 24);
    e.final = fin[0] | (fin[1] << 8) | (fin[2] << 16) | (fin[3] << 24);

    return is;
}

std::unordered_map<std::string, std::vector<ArrayElem>> ReadArrays(
    const char* arrays_filename)
{
    std::unordered_map<std::string, std::vector<ArrayElem>> arrays;
    std::string line;
    size_t offset;
    auto ifs = std::ifstream(arrays_filename);
    ArrayElem elem;

    if (!ifs.is_open()) {
        std::cerr << "Could not open file " << arrays_filename << '\n';
        return arrays;
    }

    getline(ifs, line); // skip first line (header)
    while (!ifs.eof()) {
        ifs >> elem;
        if (!elem.name.empty())
            arrays[elem.name].emplace_back(elem);
    }

    return arrays;
}
