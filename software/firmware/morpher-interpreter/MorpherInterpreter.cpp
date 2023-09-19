#include "MorpherInterpreter.h"

size_t GetLoopLength(std::unordered_map<size_t, DFGNode> dfg) {
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

size_t GetLoopIncrement(std::unordered_map<size_t, DFGNode> dfg) {
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

size_t GetLoopStart(std::unordered_map<size_t, DFGNode> dfg) {
    DFGNode select_node;

    for (auto& node : dfg) {
        if (node.second.instr == Instruction::SELECT)
            select_node = node.second;
    }

    assert(select_node.instr == Instruction::SELECT);
    assert(!select_node.inputs.empty());

    for (auto& input : select_node.inputs) {
        DFGNode cmerge_node = dfg[input];
        if (cmerge_node.instr == Instruction::CMERGE && cmerge_node.const_val != -1)
            return cmerge_node.const_val;
    }

    return 0;
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

    // std::cerr << "RunInstruction --- " << node.idx << '\n';

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

    // std::cerr << "RunInstruction --- " << node.idx << " after ifs\n";

    switch (node.instr) {
    case Instruction::SELECT:
        return loop_idx;

    case Instruction::LS:
        // std::cerr << "LS --- input1 = " << input1 << "\n";
        return input1;

    case Instruction::ADD:
        // std::cerr << "ADD --- input1 = " << input1 << "; input2 = " << input2
        //     << "; versat = " << versat << "\n";
        type = GetTypeByName(versat, MakeSizedString("ComplexAdder"));
        assert(type != nullptr);

        // std::cerr << "ADD --- type not null\n";

        accel = CreateAccelerator(versat);
        assert(accel != nullptr);
        // std::cerr << "ADD --- accel not null\n";

        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));
        assert(inst != nullptr);

        // std::cerr << "ADD --- inst not null\n";

        res = ComplexAdderInstance(accel, input1, input2);
        // std::cerr << "ADD --- res = " << res << "\n";

        return res;

    case Instruction::SHL:
        input2 = 1 << input2;

    case Instruction::MUL:
        type = GetTypeByName(versat, MakeSizedString("ComplexMultiplier"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        return ComplexMultiplierInstance(accel, input1, input2);

    case Instruction::OLOAD:
        type = GetTypeByName(versat, MakeSizedString("ComplexAdder"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        return ComplexAdderInstance(accel, arrays[node.var][0].initial, -2048) >> 2;

    case Instruction::LOAD:
        return arrays[node.var][input1].initial;

    case Instruction::STORE:
        // std::cerr << "STORE --- input1 = " << input1 << "; input2 = " << input2 <<     "\n";
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

    increment = GetLoopIncrement(dfg);
    assert(increment);

    loop_size = GetLoopLength(dfg);

    DFGNode store_node;
    for (auto& node : dfg)
        if (node.second.instr == Instruction::STORE) {
            store_node = node.second;
            break;
        }
    assert(store_node.instr == Instruction::STORE);
    assert(!store_node.inputs.empty());

    for(size_t i = GetLoopStart(dfg); i < loop_size; i += increment)
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
