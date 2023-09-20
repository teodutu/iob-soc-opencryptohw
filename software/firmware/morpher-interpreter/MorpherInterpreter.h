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

#include "pugixml.hpp"
#include "pugiconfig.hpp"

class MorpherInterpreter
{
private:
    bool nested_loop;
    int increment;
    size_t loop_size, loop_start;

    DFGNode loop_select_node;
    Versat* versat;

    std::unordered_map<std::string, std::vector<ArrayElem>> arrays;
    std::unordered_map<std::string, std::vector<ArrayElem>> initialArrays;
    std::unordered_map<size_t, DFGNode> dfg;
    std::unordered_map<size_t, int> cache;
    std::unordered_map<size_t, std::string> accumulators;
    std::vector<DFGNode> store_nodes;

    // int interpret(AddNode* node);

    int RunInstruction(size_t loop_idx, const DFGNode& node, const DFGNode& next_node);

    bool RunLoop(const char* data_location);

    bool IsNestedLoop();
    void FindLoopVariables();
    void FindAccumulators();
    void FindStoreNodes();
    void FindLoopSelectNode();

public:
    const char* dfg_file;
    const char* data_location;

    MorpherInterpreter(Versat* versat, const char* dfg_file, const char* data_location);
    ~MorpherInterpreter();

    bool Run();
};
