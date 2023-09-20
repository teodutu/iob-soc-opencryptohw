#include "MorpherInterpreter.h"

size_t MorpherInterpreter::GetLoopLength() {
    DFGNode cmp_node, add_node, select_node;

    for (auto& node : dfg) {
        cmp_node = node.second;
        if (cmp_node.instr != Instruction::CMP || cmp_node.inputs.size() != 1)
            continue;

        add_node = dfg[cmp_node.inputs[0]];
        if (add_node.instr != Instruction::ADD || add_node.inputs.size() != 1)
            continue;

        select_node = dfg[add_node.inputs[0]];
        if (select_node.instr != Instruction::SELECT)
            continue;

        return cmp_node.const_val;
    }

    return 0;
}

size_t MorpherInterpreter::GetLoopIncrement() {
    DFGNode cmp_node, add_node, select_node;

    for (auto& node : dfg) {
        cmp_node = node.second;
        if (cmp_node.instr != Instruction::CMP || cmp_node.inputs.size() != 1)
            continue;

        add_node = dfg[cmp_node.inputs[0]];
        if (add_node.instr != Instruction::ADD || add_node.inputs.size() != 1)
            continue;

        select_node = dfg[add_node.inputs[0]];
        if (select_node.instr != Instruction::SELECT)
            continue;

        return add_node.const_val;
    }

    return 0;
}

size_t MorpherInterpreter::GetLoopStart() {
    DFGNode cmerge_node, loadb_node;
    int earliest_alap = __INT_MAX__;
    size_t loop_start;

    for (auto& node : dfg)
        if (node.second.instr == Instruction::LOADB) {
            loadb_node = node.second;
            break;
        }

    assert(loadb_node.instr == Instruction::LOADB);
    assert(!loadb_node.outputs.empty());

    for (auto& output : loadb_node.outputs) {
        cmerge_node = dfg[output];
        if (cmerge_node.instr == Instruction::CMERGE && cmerge_node.alap < earliest_alap) {
            loop_start = cmerge_node.const_val;
            earliest_alap = cmerge_node.alap;
        }
    }

    return loop_start;
}

void MorpherInterpreter::FindAccumulators()
{
    std::string accumulator;
    DFGNode ostore_node, op_node, select_node;


    for (auto& node : dfg) {
        ostore_node = node.second;

        if (ostore_node.instr == Instruction::OSTORE)
            accumulator = ostore_node.var;

        if (ostore_node.inputs.empty())
            continue;

        op_node = dfg[ostore_node.inputs[0]];

        for (auto& input : op_node.inputs) {
            select_node = dfg[input];
            if (select_node.instr == Instruction::SELECT)
                accumulators[select_node.idx] = accumulator;
        }
                
    }
}

void MorpherInterpreter::FindStoreNodes()
{
    DFGNode store_node;

    for (auto& node : dfg) {
        store_node = node.second;
        if (store_node.instr == Instruction::STORE || store_node.instr == Instruction::OSTORE)
            store_nodes.emplace_back(store_node);
    }
}

MorpherInterpreter::MorpherInterpreter(Versat* versat, const char* dfg_file, const char* data_location):
    versat(versat), dfg_file(dfg_file), data_location(data_location)
{
    const std::filesystem::path path(data_location);
    std::error_code ec;

    if (std::filesystem::is_directory(path, ec))
        nested_loop = true;
    assert(!ec);

    if (std::filesystem::is_regular_file(path, ec))
        nested_loop = false;
    assert(!ec);

    dfg = ReadDFG(dfg_file);
    assert(!dfg.empty());

    increment = GetLoopIncrement();
    assert(increment);

    loop_size = GetLoopLength();
    loop_start = GetLoopStart();
    FindAccumulators();
    FindStoreNodes();
}

static int ComplexAdderInstance(Accelerator* accel,int a,int b){
    FUInstance* b1 = GetInstanceByName(accel,"Test","b1");
    FUInstance* b2 = GetInstanceByName(accel,"Test","b2");
    FUInstance* out = GetInstanceByName(accel,"Test","memOut1");

    VersatUnitWrite(b1,0,a);
    VersatUnitWrite(b2,0,b);

    ConfigureMemoryReceive(out,1,1);

    AcceleratorRun(accel);

    int result = VersatUnitRead(out,0);

    return result;
}

static int ComplexMultiplierInstance(Accelerator* accel,int a,int b){
    FUInstance* c1 = GetInstanceByName(accel,"Test","c1");
    FUInstance* c2 = GetInstanceByName(accel,"Test","c2");
    FUInstance* out = GetInstanceByName(accel,"Test","memOut2");

    VersatUnitWrite(c1,0,a);
    VersatUnitWrite(c2,0,b);

    ConfigureMemoryReceive(out,1,1);

    AcceleratorRun(accel);

    int result = VersatUnitRead(out,0);

    return result;
}

int MorpherInterpreter::RunInstruction(size_t loop_idx, const DFGNode& node) {
    Accelerator* accel;
    FUInstance *inst;
    FUDeclaration* type;
    int input1, input2, res;

    if (node.instr == Instruction::STORE && node.inputs.size() > 2) {
        input1 = RunInstruction(loop_idx, dfg[node.inputs[0]]);
        input2 = RunInstruction(loop_idx, dfg[node.inputs[1]]);
    } else if (node.instr != Instruction::SELECT && !node.inputs.empty()) {
        input1 = RunInstruction(loop_idx, dfg[node.inputs[0]]);

        if (node.instr == Instruction::ADD && node.offset >= 0)
            input2 = node.offset;
        else if (node.inputs.size() > 1)
            input2 = RunInstruction(loop_idx, dfg[node.inputs[1]]);
        else
            input2 = node.const_val;
    }

    switch (node.instr) {
    case Instruction::SELECT:
        if (node.inputs.size() > 2)
            return arrays[accumulators[node.idx]][0].initial;

        return loop_idx;

    case Instruction::LS:
        return input1;

    case Instruction::ADD:
        type = GetTypeByName(versat, MakeSizedString("ComplexAdder"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        assert(accel != nullptr);

        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));
        assert(inst != nullptr);

        res = ComplexAdderInstance(accel, input1, input2);
        return res;

    case Instruction::SHL:
        input2 = 1 << input2;

    case Instruction::MUL:
        type = GetTypeByName(versat, MakeSizedString("ComplexMultiplier"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        res = ComplexMultiplierInstance(accel, input1, input2);
        return res;

    case Instruction::OLOAD:
        type = GetTypeByName(versat, MakeSizedString("ComplexAdder"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        res = ComplexAdderInstance(accel, arrays[node.var][0].initial, -2048) >> 2;
        return res;

    case Instruction::LOAD:
        return arrays[node.var][input1].initial;

    case Instruction::OSTORE:
        arrays[node.var][0].initial = input1;
        return input1;

    case Instruction::STORE:
        arrays[node.var][input1].initial = input2;
        return input2;

    default:
        std::cerr << "Unsupported operation " << static_cast<int>(node.instr) << '\n';
        assert(0);
    }
}

bool MorpherInterpreter::Run()
{
    if (!nested_loop)
        return RunLoop(data_location);

    for (const auto& dir_entry :
            std::filesystem::recursive_directory_iterator(data_location)) {
        auto res = RunLoop(dir_entry.path().c_str());
        if (!res)
            return false;
    }

    return true;
}

bool MorpherInterpreter::RunLoop(const char* data_file)
{
    arrays = ReadArrays(data_file);
    assert(!arrays.empty());

    arrays.erase("loopstart");
    arrays.erase("loopend");
    initialArrays = arrays;

    for (size_t i = loop_start; i < loop_size; i += increment)
        for (auto& store_node : store_nodes)
            RunInstruction(i, store_node);

    for (auto& array : arrays)
        for (size_t i = 0; i < array.second.size(); i++)
            if (array.second[i].initial != array.second[i].final) {
                std::cout << "ERROR - " << array.first << "[" << i << "]: "
                    << array.second[i].initial << " != " << array.second[i].final << '\n';
                return false;
            }

    return true;
}

MorpherInterpreter::~MorpherInterpreter()
{
}
