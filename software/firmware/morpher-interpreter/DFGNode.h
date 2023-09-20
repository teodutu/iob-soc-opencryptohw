#ifndef DFG_NODE_H_
#define DFG_NODE_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cassert>

#include <string.h>

// #include "MorpherInterpreter.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"

enum class Instruction
{
    ERR = 0,
    ADD,
    SUB,
    MUL,
    DIV,
    SHL,
    SHR,
    LOAD,
    OLOAD,
    STORE,
    OSTORE,
    LOADB,
    STOREB,
    LS,
    CMERGE,
    MOVC,
    CMP,
    SELECT,
};

struct DFGNode
{
    Instruction instr;
    int const_val, offset, asap, alap;
    size_t idx;
    std::string var, bb;
    std::vector<size_t> inputs;
    std::vector<size_t> outputs;

    // virtual int accept(MorpherInterpreter* interpreter) = 0;
};

// class AddNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class MulNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class DivNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class ShlNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class ShrNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class LoadNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class OLoadNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class StoreNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class LoadBNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class StoreBNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class LSNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class CMergeNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class MovCNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class CMPNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

// class SelectNode : DFGNode
// {
//     int accept(MorpherInterpreter& interpreter) override;
// };

std::unordered_map<size_t, DFGNode> ReadDFG(const char* xml_file);

#endif  // DFG_NODE_H_
