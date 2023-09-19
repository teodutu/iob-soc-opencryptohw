#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include <string.h>

#include "DFGNode.h"
#include "ArrayElem.h"

#include "versat.hpp"
#include "utils.hpp"
#include "unitConfiguration.hpp"
#include "verilogWrapper.inc"
#include "versatSHA.hpp"
#include "versatAES.hpp"

#include "pugixml.hpp"
#include "pugiconfig.hpp"

class MorpherInterpreter
{
private:
    bool nested_loop;

    int accumulator;
    int increment;
    size_t loop_size;

    Versat* versat;

    std::unordered_map<std::string, std::vector<ArrayElem>> arrays;
    std::unordered_map<std::string, std::vector<ArrayElem>> initialArrays;
    std::unordered_map<size_t, DFGNode> dfg;
    std::unordered_map<size_t, int> cache; 

    // int interpret(AddNode* node);

    int RunInstruction(size_t loop_idx, const DFGNode& node);

    bool RunLoop(const char* data_location);

public:
    const char* dfg_file;
    const char* data_location;

    MorpherInterpreter(Versat* versat, const char* dfg_file, const char* data_location);
    ~MorpherInterpreter();

    bool Run();
};

size_t GetLoopLength(std::unordered_map<size_t, DFGNode> dfg);

size_t GetLoopIncrement(std::unordered_map<size_t, DFGNode> dfg);

size_t GetLoopStart(std::unordered_map<size_t, DFGNode> dfg);
