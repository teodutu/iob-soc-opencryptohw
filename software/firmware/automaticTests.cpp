#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>

#include "versat.hpp"
#include "versatExtra.hpp"
#include "utils.hpp"
#include "unitConfiguration.hpp"
#include "verilogWrapper.inc"
#include "basicWrapper.inc"
#include "versatSHA.hpp"
#include "versatAES.hpp"

#ifdef USE_MORPHER
#include <sstream>

#include "pugixml.hpp"
#include "pugiconfig.hpp"
#endif // USE_MORPHER

extern "C"{
#include "../test_vectors.h"
//#include "crypto/sha2.h"

int printf_(const char* format, ...);
}

#ifndef PC
#define printf printf_
#endif

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Disable accelerator running in PC if doing an hardware test
#if 0
#if defined(HARDWARE_TEST) && defined(PC)
#define AcceleratorRun(...) ((void)0)
#endif
#endif

struct TestInfo{
    int testsPassed;
    int numberTests;

    TestInfo(int passed, int numberTests = 1):testsPassed(passed),numberTests(numberTests){};

    TestInfo& operator+=(TestInfo t){
        testsPassed += t.testsPassed;
        numberTests += t.numberTests;

        return *this;
    }
};

#define TEST_FAILED(REASON) do{ printf("\n[%2d]Test failed: %s\n\t%s\n\n",testNumber,__PRETTY_FUNCTION__,REASON); return TestInfo(0);} while(0)
#define TEST_PASSED return TestInfo(1)

#define FORCE_FAIL 0

// Care with the testNumber variable. Every test must have one
#define EXPECT(...) Expect_(__PRETTY_FUNCTION__,testNumber,__VA_ARGS__)
static TestInfo Expect_(const char* functionName,int testNumber, const char* expected,const char* format, ...) __attribute__ ((format (printf, 4, 5)));

static TestInfo Expect_(const char* functionName,int testNumber, const char* expected,const char* format, ...){
    va_list args;
    va_start(args,format);

    char buffer[1024];
    int size = vsprintf(buffer,format,args);
    Assert(size < 1024);

    va_end(args);

    #if FORCE_FAIL
        expected = "";
    #endif

    bool result = (strcmp(expected,buffer) == 0);
    if(result){
        TEST_PASSED;
    } else {
        printf("\n");
        printf("[%2d]Test failed: %s\n",testNumber,functionName);
        printf("    Expected: %s\n",expected);
        printf("    Result:   %s\n",buffer);
        printf("              ");
        for(int i = 0; expected[i] != '\0'; i++){
            if(buffer[i] == '\0'){
                printf("^");
                break;
            }
            if(buffer[i] != expected[i]){
                printf("^");
            } else {
                printf(" ");
            }
        }

        printf("\n");

        return TestInfo(0);
    }
}

#define TEST(TEST_NAME) static TestInfo TEST_NAME(Versat* versat,int testNumber)
#define TEST_FILE(TEST_NAME) static TestInfo TEST_NAME(Versat* versat, int testNumber, \
    const char* dfg_file, const char* data_file)

#include <cstdlib>

#define SYS_N (3488)
#define VEC_SZ (SYS_N / 8) // 436
// 16, 64 works
// 3488 / 8 = 436 does not work
// 128 seems to hang
// #define VEC_SZ (132)
TEST(VectorLikeOperation){
   uint32_t mat[VEC_SZ/4] = {0};
   uint32_t row[VEC_SZ/4] = {0};
   uint32_t expected[VEC_SZ/4] = {0};
   uint32_t result[VEC_SZ/4] = {0};
   uint32_t mask = 0xFFFFFFFF;
   int i = 0;
   int n_cols = VEC_SZ / 4;
   for(i=0; i<n_cols;i++){
       mat[i] = (uint32_t) i*4;
       row[i] = (uint32_t) 0xFF;
       for(int j=1; j<4; j++){
           mat[i] = (mat[i] << 8) | (i*4 + j);
           row[i] = (row[i] << 8) | 0xFF;
       }
       expected[i] = ~(mat[i]);
   }

   FUDeclaration* type = GetTypeByName(versat,STRING("VectorLikeOperation"));
   Accelerator* accel = CreateAccelerator(versat);
   FUInstance* inst = CreateFUInstance(accel,type,STRING("Test"));

   // printf("config vread\n");
   ConfigureSimpleVRead(GetInstanceByName(accel,"Test","row"), VEC_SZ / 4,(int*) row);

   // printf("config mem mat\n");
   FUInstance* matInst = GetInstanceByName(accel,"Test","mat");
   ConfigureMemoryLinear(matInst, VEC_SZ / 4);
   for (int c = 0; c < VEC_SZ / 4; c++){
       VersatUnitWrite(matInst,c,mat[c]);
   return EXPECT("7","%d",result);
}

int ComplexAdderInstance(Accelerator* accel,int a,int b){
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

int ComplexMultiplierInstance(Accelerator* accel,int a,int b){
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

int SemiComplexAdderInstance(Accelerator* accel,int a,int b){
    FUInstance* d1 = GetInstanceByName(accel,"Test","d1");
    FUInstance* d2 = GetInstanceByName(accel,"Test","d2");
    FUInstance* out = GetInstanceByName(accel,"Test","memOut3");

    d1->config[0] = a;
    VersatUnitWrite(d2,0,b);

    ConfigureMemoryReceive(out,1,1);

    AcceleratorRun(accel);

    int result = VersatUnitRead(out,0);

    return result;
}

TEST(ComplexMultiplier){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("ComplexMultiplier"));
    Accelerator* accel = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel,type,MakeSizedString("Test"));

    int result = ComplexMultiplierInstance(accel,4,5);

    return EXPECT("20","%d",result);
}

TEST(SimpleShareConfig){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("SimpleShareConfig"));
    Accelerator* accel = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel,type,MakeSizedString("Test"));

    FUInstance* a1 = GetInstanceByName(accel,"Test","a1");
    FUInstance* a2 = GetInstanceByName(accel,"Test","a2");
    FUInstance* b1 = GetInstanceByName(accel,"Test","b1");
    FUInstance* b2 = GetInstanceByName(accel,"Test","b2");
    FUInstance* out0 = GetInstanceByName(accel,"Test","out0");
    FUInstance* out1 = GetInstanceByName(accel,"Test","out1");
    FUInstance* out2 = GetInstanceByName(accel,"Test","out2");

    a1->config[0] = 2;
    AcceleratorRun(accel);
    int res0 = out0->state[0];

    a1->config[0] = 0;
    a2->config[0] = 3;
    AcceleratorRun(accel);
    int res1 = out0->state[0];

    b2->config[0] = 4;
    AcceleratorRun(accel);
    int res2 = out1->state[0];

    a1->config[0] = 0;
    a2->config[0] = 0;
    b1->config[0] = 0;
    b2->config[0] = 0;

    a2->config[0] = 3;
    b2->config[0] = 4;
    AcceleratorRun(accel);
    int res3 = out2->state[0];

    return EXPECT("4 6 8 7","%d %d %d %d",res0,res1,res2,res3);
}

TEST(ComplexShareConfig){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("ComplexShareConfig"));
    Accelerator* accel = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel,type,MakeSizedString("Test"));

    // Test by changing config for shared 1
    FUInstance* a11 = GetInstanceByName(accel,"Test","shared1","a1");
    FUInstance* a12 = GetInstanceByName(accel,"Test","shared1","a2");
    FUInstance* b11 = GetInstanceByName(accel,"Test","shared1","b1");
    FUInstance* b12 = GetInstanceByName(accel,"Test","shared1","b2");

    // But reading the output of shared 2 (should be the same, since same configuration = same results)
    FUInstance* out20 = GetInstanceByName(accel,"Test","shared2","out0");
    FUInstance* out21 = GetInstanceByName(accel,"Test","shared2","out1");
    FUInstance* out22 = GetInstanceByName(accel,"Test","shared2","out2");

    a11->config[0] = 2;
    AcceleratorRun(accel);
    int res0 = out20->state[0];

    a11->config[0] = 0;
    a12->config[0] = 3;
    AcceleratorRun(accel);
    int res1 = out20->state[0];

    b12->config[0] = 4;
    AcceleratorRun(accel);
    int res2 = out21->state[0];

    a11->config[0] = 0;
    a12->config[0] = 0;
    b11->config[0] = 0;
    b12->config[0] = 0;

    a12->config[0] = 3;
    b12->config[0] = 4;
    AcceleratorRun(accel);
    int res3 = out22->state[0];

    return EXPECT("4 6 8 7","%d %d %d %d",res0,res1,res2,res3);
}

TEST(SimpleFlatten){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("SimpleAdder"));
    Accelerator* accel = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel,type,MakeSizedString("Test"));

    Accelerator* flatten = Flatten(versat,accel,1);

    int result = SimpleAdderInstance(flatten,4,5);

    return EXPECT("9","%d",result);
}

TEST(FlattenShareConfig){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("ComplexShareConfig"));
    Accelerator* accel_ = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel_,type,MakeSizedString("Test"));

    Accelerator* flatten = Flatten(versat,accel_,99);

    // Test by changing config for shared 1
    FUInstance* a11 = GetInstanceByName(flatten,"Test","shared1","a1");
    FUInstance* a12 = GetInstanceByName(flatten,"Test","shared1","a2");
    FUInstance* b11 = GetInstanceByName(flatten,"Test","shared1","b1");
    FUInstance* b12 = GetInstanceByName(flatten,"Test","shared1","b2");

    // But reading the output of shared 2 (should be the same, since same configuration = same results)
    FUInstance* out20 = GetInstanceByName(flatten,"Test","shared2","out0");
    FUInstance* out21 = GetInstanceByName(flatten,"Test","shared2","out1");
    FUInstance* out22 = GetInstanceByName(flatten,"Test","shared2","out2");

    a11->config[0] = 2;
    AcceleratorRun(flatten);
    int res0 = out20->state[0];

    a11->config[0] = 0;
    a12->config[0] = 3;
    AcceleratorRun(flatten);
    int res1 = out20->state[0];

    b12->config[0] = 4;
    AcceleratorRun(flatten);
    int res2 = out21->state[0];

    a11->config[0] = 0;
    a12->config[0] = 0;
    b11->config[0] = 0;
    b12->config[0] = 0;

    a12->config[0] = 3;
    b12->config[0] = 4;
    AcceleratorRun(flatten);
    int res3 = out22->state[0];

    return EXPECT("4 6 8 7","%d %d %d %d",res0,res1,res2,res3);
}

TEST(FlattenSHA){
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("SHA"));
    Accelerator* accel = CreateAccelerator(versat);
    FUInstance* inst = CreateFUInstance(accel,type,MakeSizedString("Test"));

    Accelerator* flatten = Flatten(versat,accel,99);

    SetSHAAccelerator(flatten,nullptr);

    InitVersatSHA(versat,true);

    unsigned char digest[256];
    for(int i = 0; i < 256; i++){
        digest[i] = 0;
    }

   // printf("config mask\n");
   FUInstance* maskInst = GetInstanceByName(accel,"Test","mask");
   maskInst->config[0] = mask;

   // printf("config output\n");
   FUInstance* outputInst = GetInstanceByName(accel,"Test","output");
   // ConfigureMemoryReceive(outputInst, VEC_SZ / 4, 1);
   // ConfigureMemoryLinearOut(outputInst, VEC_SZ / 4);
   ConfigureMemoryLinearOut(outputInst, VEC_SZ / 4);
   
   // printf("accel 1\n");
   AcceleratorRun(accel); // Fills vread with valid data
   // printf("accel 2\n");
   AcceleratorRun(accel);

   // printf("read results\n");
   for (int c = 0; c < VEC_SZ/4; c++){
        result[c] = VersatUnitRead(outputInst,c);
   }
   
   // printf("output versat\n");
   OutputVersatSource(versat,accel,".");

   // printf("cmp results\n");
   if (memcmp(result, expected, VEC_SZ/4) == 0) {
       printf("Test Passed\n");
       TEST_PASSED;
   } else {
       printf("Input:\n");
       for(int i = 0; i < VEC_SZ/4; i++){
          printf("0x%08x ", mat[i]);
       }
       printf("\nResult:\n");
       for(int i = 0; i < VEC_SZ/4; i++){
          printf("0x%08x ", result[i]);
       }
       printf("\n\nExpected:\n");
       for(int i = 0; i < VEC_SZ/4; i++){
          printf("0x%08x ", expected[i]);
       }
       TEST_FAILED("Result differ from expected value");
   }

}

TEST(SHA){
   FUDeclaration* type = GetTypeByName(versat,STRING("SHA"));
   Accelerator* accel = CreateAccelerator(versat);
   FUInstance* inst = CreateFUInstance(accel,type,STRING("Test"));

    SetSHAAccelerator(accel,inst);

    InitVersatSHA(versat,true);

    unsigned char digest[256];
    for(int i = 0; i < 256; i++){
        digest[i] = 0;
    }

    VersatSHA(digest,msg_64,64);

    //printf("%s\n",buffer);

    return EXPECT("42e61e174fbb3897d6dd6cef3dd2802fe67b331953b06114a65c772859dfc1aa","%s",GetHexadecimal(digest, HASH_SIZE));
}

TEST(AESWithIterative){
   SimpleAccelerator test = {};
   InitSimpleAccelerator(&test,versat,"AES256WithIterative");

   FUInstance* t = GetInstanceByName(test.accel,"Test","mk0","roundPairAndKey");
   FUInstance* s = GetInstanceByName(test.accel,"Test","subBytes");
   FUInstance* k = GetInstanceByName(test.accel,"Test","key6");
   FUInstance* r = GetInstanceByName(test.accel,"Test","round0");

   FillSubBytes(test.accel,s);
   FillKeySchedule256(test.accel,k);
   FillRound(test.accel,r);

   FUInstance* merge = GetInstanceByName(test.accel,"Test","mk0","Merge0");
   merge->config[0] = 8;

   int rcon[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};
   for(int i = 0; i < 7; i++){
      FUInstance* inst = GetInstanceByName(test.accel,"Test","rcon%d",i);
      inst->config[0] = rcon[i];
   }

   FillRoundPairAndKey(test.accel,t);

   int* out = RunSimpleAccelerator(&test, 0xcc,0xc6,0x2c,0x6b,
                                          0x0a,0x09,0xa6,0x71,
                                          0xd6,0x44,0x56,0x81,
                                          0x8d,0xb2,0x9a,0x4d,
                                          0xcc,0x22,0xda,0x78,
                                          0x7f,0x37,0x57,0x11,
                                          0xc7,0x63,0x02,0xbe,
                                          0xf0,0x97,0x9d,0x8e,
                                          0xdd,0xf8,0x42,0x82,
                                          0x9c,0x2b,0x99,0xef,
                                          0x3d,0xd0,0x4e,0x23,
                                          0xe5,0x4c,0xc2,0x4b);

   OutputVersatSource(versat,&test,".");

   char buffer[1024];
   char* ptr = buffer;
   for(int i = 0; i < 16; i++){
      ptr += sprintf(ptr,"0x%02x ",out[i]);
   }

   return EXPECT("0xdf 0x86 0x34 0xca 0x02 0xb1 0x3a 0x12 0x5b 0x78 0x6e 0x1d 0xce 0x90 0x65 0x8b ","%s",buffer);
}

// #ifdef USE_MORPHER
struct ArrayElem {
    int idx, initial, final;
    std::string name;
};

static int ReadNextToken(std::stringstream& s, std::string token) {
    getline(s, token, ',');
    // std::cout << token << '\n';
    return std::stoi(token);    
}

std::istream& operator>>(std::istream& is, ArrayElem& e) {
    const size_t size = sizeof(e.initial);
    std::string line, token;
    int init[4], fin[4];
    char delim;

    for (size_t i = 0; i < size; i++) {
        getline(is, line);
        std::stringstream s(line);

        getline(s, e.name, ',');
        if (e.name[0] == '\n')
            e.name.erase(0, 1);

        // std::cout << "name = " << e.name << '; \n';
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
    const char* arrays_filename) {
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

pugi::xml_node GeXMLDFG(const char* xml_file)
{
    pugi::xml_document doc;
    pugi::xml_parse_result xml_result = doc.load_file(xml_file);

    if (!xml_result) {
        std::cout << "XML [" << xml_file << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << xml_result.description() << "\n";
        std::cout << "Error offset: " << xml_result.offset << " (error at offset [" << xml_result.offset << "]\n\n";

        return pugi::xml_node();
    }

    return doc.child("xml").child("DFG");
}

enum class Instruction {
    ERR = 0,
    ADD,
    SUB,
    MUL,
    DIV,
    SHL,
    SHR,
    LOAD,
    STORE,
    LOADB,
    STOREB,
    LS,
    CMERGE,
    MOVC,
    CMP,
    SELECT,
};

static Instruction CreateInstruction(const char* instruction_str) {
    if (strcmp(instruction_str, "ADD") == 0)
        return Instruction::ADD;
    else if (strcmp(instruction_str, "MUL") == 0)
        return Instruction::MUL;
    else if (strcmp(instruction_str, "DIV") == 0)
        return Instruction::DIV;
    else if (strcmp(instruction_str, "SHL") == 0)
        return Instruction::SHL;
    else if (strcmp(instruction_str, "SHR") == 0)
        return Instruction::SHR;
    else if (strcmp(instruction_str, "LOAD") == 0)
        return Instruction::LOAD;
    else if (strcmp(instruction_str, "STORE") == 0)
        return Instruction::STORE;
    else if (strcmp(instruction_str, "LOADB") == 0)
        return Instruction::LOADB;
    else if (strcmp(instruction_str, "STOREB") == 0)
        return Instruction::STOREB;
    else if (strcmp(instruction_str, "LS") == 0)
        return Instruction::LS;
    else if (strcmp(instruction_str, "CMERGE") == 0)
        return Instruction::CMERGE;
    else if (strcmp(instruction_str, "MOVC") == 0)
        return Instruction::MOVC;
    else if (strcmp(instruction_str, "CMP") == 0)
        return Instruction::CMP;
    else if (strcmp(instruction_str, "SELECT") == 0)
        return Instruction::SELECT;
    else {
        std::cerr << "Invalid instruction: " << instruction_str << '\n';
        assert(false);
    }
}

struct InstructionNode {
    Instruction instr;
    int const_val, offset;
    size_t idx;
    std::string var;
    std::vector<size_t> inputs;
    std::vector<size_t> outputs;
};

std::unordered_map<size_t, InstructionNode> ReadDFG(const char* xml_file)
{
    std::unordered_map<size_t, InstructionNode> dfg;
    pugi::xml_node pugi_dfg = GeXMLDFG(xml_file);
    std::string var_name;
    size_t const_val;
    
    if (!pugi_dfg) {
        std::cerr << "Could not parse XML file " << xml_file << '\n';
        return dfg;
    }

    for (pugi::xml_node node = pugi_dfg.child("Node"); node; node = node.next_sibling("Node")) {
        InstructionNode instr_node;
        instr_node.idx = node.attribute("idx").as_ullong();

        instr_node.instr = CreateInstruction(node.child_value("OP"));
        instr_node.var = node.child_value("BasePointerName");
        instr_node.const_val = node.attribute("CONST").as_int();

        std::string offset_str = node.child_value("GEPOffset");
        if (!offset_str.empty())
            instr_node.offset = std::stoi(offset_str);
        else
            instr_node.offset = -1;

        for (pugi::xml_node input = node.child("Inputs").child("Input"); input;
                input = input.next_sibling("Input"))
            instr_node.inputs.push_back(input.attribute("idx").as_ullong());

        for (pugi::xml_node output = node.child("Outputs").child("Output"); output;
                output = output.next_sibling("Output"))
            instr_node.outputs.push_back(output.attribute("idx").as_ullong());

        dfg[instr_node.idx] = instr_node;
    }

    return dfg;
}

static InstructionNode GetSelectNode(std::unordered_map<size_t, InstructionNode> dfg) {
    for (auto& node : dfg) {
        if (node.second.instr == Instruction::SELECT) {
            return node.second;
        }
    }

    assert(false);
}

static InstructionNode FindOutputByOp(std::unordered_map<size_t, InstructionNode> dfg,
        InstructionNode node, Instruction op) {
    for (auto& output : node.outputs) {
        InstructionNode out_node = dfg[output];
        if (out_node.instr == op)
            return out_node;
    }

    return InstructionNode();
}

static size_t GetLoopLength(std::unordered_map<size_t, InstructionNode> dfg) {
    InstructionNode select_node = GetSelectNode(dfg);
    assert(select_node.instr == Instruction::SELECT);
    assert(!select_node.outputs.empty());

    InstructionNode add_node = FindOutputByOp(dfg, select_node, Instruction::ADD);
    assert(add_node.instr == Instruction::ADD);

    InstructionNode cmp_node = FindOutputByOp(dfg, add_node, Instruction::CMP);
    assert(cmp_node.instr == Instruction::CMP);

    return cmp_node.const_val;

    for (auto& sel_output : select_node.outputs) {
        InstructionNode cmp_node = dfg[sel_output];
        if (cmp_node.instr == Instruction::ADD)
            for (auto& cmp_output : cmp_node.outputs) {
                InstructionNode loop_node = dfg[cmp_output];
                if (loop_node.instr == Instruction::CMP)
                    return loop_node.const_val;
            }
    }

    return 0;
}

static size_t GetLoopIncrement(std::unordered_map<size_t, InstructionNode> dfg) {
    InstructionNode select_node = GetSelectNode(dfg);
    assert(select_node.instr == Instruction::SELECT);
    assert(!select_node.outputs.empty());

    InstructionNode add_node = FindOutputByOp(dfg, select_node, Instruction::ADD);
    assert(add_node.instr == Instruction::ADD);

    return add_node.const_val;
}

static int RunInstruction(Versat *versat,
        std::unordered_map<std::string, std::vector<ArrayElem>>& arrays,
        size_t loop_idx, std::unordered_map<size_t, InstructionNode>& dfg,
        const InstructionNode& node) {
    Accelerator* accel;
    FUInstance *inst;
    FUDeclaration* type;
    int input1, input2;

    assert(!node.inputs.empty());

    if (node.instr != Instruction::LS) {
        input1 = RunInstruction(versat, arrays, loop_idx, dfg, dfg.at(node.inputs[0]));

        if (node.instr == Instruction::ADD && node.offset >= 0)
            input2 = node.offset;
        else if (node.inputs.size() > 1)
            input2 = RunInstruction(versat,arrays, loop_idx, dfg, dfg.at(node.inputs[1]));
        else
            input2 = node.const_val;
    }

    switch (node.instr) {
    case Instruction::LS:
        return loop_idx;

    case Instruction::ADD:
        type = GetTypeByName(versat, MakeSizedString("ComplexAdder"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        return ComplexAdderInstance(accel, input1, input2);

    case Instruction::SHL:
        input2 = 1 << input2;

    case Instruction::MUL:
        type = GetTypeByName(versat, MakeSizedString("ComplexMultiplier"));
        assert(type != nullptr);

        accel = CreateAccelerator(versat);
        inst = CreateFUInstance(accel, type, MakeSizedString("Test"));

        return ComplexMultiplierInstance(accel, input1, input2);

    case Instruction::LOAD:
        return arrays[node.var][input1].initial;

    case Instruction::STORE:
        arrays[node.var][input1].initial = input2;
        return input2;

    default:
        std::cerr << "Unsupported operation " << static_cast<int>(node.instr) << '\n';
        assert(0);
    }
}

TEST_FILE(SimpleOperation){
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::milliseconds elapsed;
    std::unordered_map<std::string, std::vector<ArrayElem>> initialArrays;
    int res;
    size_t size = 0, incr = 0;

    auto arrays = ReadArrays(data_file);
    initialArrays = arrays;
    auto xml_dfg = ReadDFG(dfg_file);

    arrays.erase("loopstart");
    arrays.erase("loopend");

    assert(!arrays.empty());
    assert(!xml_dfg.empty());

    incr = GetLoopIncrement(xml_dfg);
    assert(incr);

    size = GetLoopLength(xml_dfg);
    for (auto& array : arrays)
        assert(array.second.size() == size);

    size = 20;

    InstructionNode store_node;
    for (auto& node : xml_dfg)
        if (node.second.instr == Instruction::STORE) {
            store_node = node.second;
            break;
        }
    assert(store_node.instr == Instruction::STORE);
    assert(!store_node.inputs.empty());

    start = std::chrono::high_resolution_clock::now();

    for(size_t i = 0; i < size; i += incr)
        RunInstruction(versat, arrays, i, xml_dfg, store_node);

    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Elapsed time: " << elapsed.count() << "ms\n";

    for (auto& array : arrays)
        for (size_t i = 0; i < size; i++)
            if (array.second[i].initial != array.second[i].final) {
                std::cout << "Error: arr = " << array.first << "; i = " << i << ": "
                    << array.second[i].initial << " != " << array.second[i].final << '\n';
                return TestInfo(0);
            }
    

    return TestInfo(1); 
}

TEST(DotProduct){
    Accelerator* accel = CreateAccelerator(versat);
    FUDeclaration* type = GetTypeByName(versat,MakeSizedString("MatrixMultiplication"));
    CreateFUInstance(accel,type,MakeSizedString("test"));

    FUInstance* memA = GetInstanceByName(accel,"test","matA");
    FUInstance* memB = GetInstanceByName(accel,"test","matB");
    FUInstance* muladd = GetInstanceByName(accel,"test","ma");

    FUInstance* res = GetInstanceByName(accel,"test","res");

    auto arrays = ReadArrays("array_prod_trace.csv");
    int dimensions = arrays["A"].size();
    int size = dimensions;

    ConfigureLeftSideMatrix(memA, dimensions);
    ConfigureLeftSideMatrix(memB, dimensions);

    for(int i = 0; i < size; i++){
        VersatUnitWrite(memA, i, arrays["A"][i].initial);
        VersatUnitWrite(memB, i, arrays["B"][i].initial);
    }

    volatile MuladdConfig* conf = (volatile MuladdConfig*) muladd->config;

    conf->opcode = 0;
    conf->iterations = size;
    conf->period = size;
    conf->shift = 0;

    ConfigureMemoryReceive(res, 1, 1);

    AcceleratorRun(accel);

    OutputVersatSource(versat,accel,"versat_instance.v","versat_defs.vh","versat_data.inc");

    char result[1024];
    char* ptrRes = result;
    for(int i = 0; i < dimensions; i++){
        ptrRes += sprintf(ptrRes, "%d ",VersatUnitRead(res, i));
    }

    return EXPECT("30 ", "%s", result);
}

TEST(DisplayMorpherDFG){
    const char* source = "ArrayAdd.xml";
    pugi::xml_node xml_dfg = GeXMLDFG(source);

    std::unordered_map<int, pugi::xml_node> nodes;
    std::unordered_map<int, std::vector<int>> dfg;

    for (pugi::xml_node node = xml_dfg.child("Node"); node; node = node.next_sibling("Node"))
    {
        int idx = node.attribute("idx").as_int();
        std::cout << "Node " << idx << "\n";
        std::cout << "\tOP " << node.child_value("OP") << "\n";
        std::cout << "\tASAP " << node.attribute("ASAP").as_int() << "\n";
        std::cout << "\tALAP " << node.attribute("ALAP").as_int() << "\n";
        std::cout << "\tBB " << node.attribute("BB").value() << "\n";
        std::cout << "\tCONST " << node.attribute("CONST").as_int() << "\n";

        pugi::xml_node inputs = node.child("Inputs");
        std::cout << "\tInputs:\n";
        for (pugi::xml_node input = inputs.child("Input"); input; input = input.next_sibling("Input"))
        {
            std::cout << "\t\tInput " << input.attribute("idx").as_int() << "\n";
        }

        pugi::xml_node outputs = node.child("Outputs");
        std::cout << "\tOutputs:\n";
        for (pugi::xml_node output = outputs.child("Output"); output; output = output.next_sibling("Output"))
        {
            int output_idx = output.attribute("idx").as_int();
            dfg[idx].push_back(output_idx);

            std::cout << "\t\tOutput " << output_idx << "\n";
            std::cout << "\t\t\tnextiter " << output.attribute("nextiter").as_int() << "\n";
            std::cout << "\t\t\ttype " << output.attribute("type").value() << "\n";
        }

        nodes[node.attribute("idx").as_int()] = node;
    }

    for (auto& neighbours : dfg)
    {
        std::cout << nodes[neighbours.first].child_value("OP") << "(" << neighbours.first << ") ->: ";
        for (auto& node : neighbours.second)
        {
            std::cout << nodes[node].child_value("OP") << "(" << node << ") ";
        }
        std::cout << "\n";
    }

    Accelerator* accel = CreateAccelerator(versat);

    return EXPECT("1", "%d", 1);
}
#endif // USE_MORPHER

// When 1, need to pass 0 to enable test (changes enabler from 1 to 0)
#define REVERSE_ENABLED 0

#define DISABLED (REVERSE_ENABLED)

#undef HARDWARE_TEST

#ifndef HARDWARE_TEST
    #define HARDWARE_TEST -1
    #define ENABLE_TEST(ENABLED) (!(ENABLED) != !(REVERSE_ENABLED))
#else
    #define ENABLE_TEST(ENABLED) (currentTest == hardwareTest)
#endif

#define TEST_INST(ENABLED, TEST_NAME)                                   \
    do {                                                                \
        if (ENABLE_TEST(ENABLED)) {                                     \
            TestInfo test = TEST_NAME(versat, currentTest);             \
            if (test.testsPassed == test.numberTests)                   \
                printf("%32s [%02d] - OK\n", #TEST_NAME, currentTest);  \
            info += test;                                               \
        }                                                               \
        currentTest += 1;                                               \
    } while (0)

#define TEST_INST_FILE(ENABLED, TEST_NAME)                                          \
    do {                                                                            \
        if (ENABLE_TEST(ENABLED)) {                                                 \
            TestInfo test = TEST_NAME(versat, currentTest, #TEST_NAME ".xml",       \
                #TEST_NAME ".csv");                                                 \
            if (test.testsPassed == test.numberTests)                               \
                printf("%32s [%02d] - OK\n", #TEST_NAME, currentTest);              \
            info += test;                                                           \
        }                                                                           \
        currentTest += 1;                                                           \
    } while (0)

#define TEST_INST_OPERATION(ENABLED, TEST_NAME, OPERATION)                          \
    do {                                                                            \
        if (ENABLE_TEST(ENABLED)) {                                                 \
            TestInfo test = TEST_NAME(versat, currentTest, OPERATION".xml",         \
                OPERATION".csv");                                                   \
            if (test.testsPassed == test.numberTests)                               \
                printf("%32s [%02d] - OK\n", #OPERATION, currentTest);              \
            info += test;                                                           \
        }                                                                           \
        currentTest += 1;                                                           \
    } while (0)


void AutomaticTests(Versat* versat){
    TestInfo info = TestInfo(0,0);
    int hardwareTest = HARDWARE_TEST;
    int currentTest = 0;

#ifdef USE_MORPHER
    TEST_INST(0, DisplayMorpherDFG);
    TEST_INST_OPERATION(1, SimpleOperation, "ArrayAdd");
    TEST_INST_OPERATION(1, SimpleOperation, "ElemProd");

    // TEST_INST(1, DotProduct);
#endif  // USE_MORPHER

#if 0
#if 1
    TEST_INST( 1 ,TestMStage);
    TEST_INST( 1 ,TestFStage);
    TEST_INST( 1 ,SHA);
    TEST_INST( 1 ,MultipleSHATests);
#endif
#if 1
    TEST_INST( 1 ,VReadToVWrite);
    TEST_INST( 0 ,StringHasher);
    TEST_INST( 1 ,Convolution);
    TEST_INST( 1 ,MatrixMultiplication);
    TEST_INST( 1 ,MatrixMultiplicationVRead);
    TEST_INST( 1 ,VersatAddRoundKey);
    TEST_INST( 1 ,LookupTable);
    TEST_INST( 1 ,VersatSubBytes);
    TEST_INST( 1 ,VersatShiftRows);
#endif
#if 1
    TEST_INST( 1 ,VersatDoRows);
    TEST_INST( 1 ,VersatMixColumns);
    TEST_INST( 1 ,FirstLineKey);
    TEST_INST( 1 ,KeySchedule);
    TEST_INST( 1 ,AESRound);
    TEST_INST( 0 ,AES);
    TEST_INST( 1 ,ReadWriteAES);
    TEST_INST( 1 ,SimpleAdder);
    TEST_INST( 1 ,ComplexMultiplier);
#endif
#if 1
    TEST_INST( 1 ,SimpleShareConfig);
    TEST_INST( 1 ,ComplexShareConfig);
#endif
#if 0
    TEST_INST( 1 ,SimpleFlatten);
    TEST_INST( 1 ,FlattenShareConfig);
    TEST_INST( 1 ,ComplexFlatten);
    TEST_INST( 1 ,FlattenSHA); // Problem on top level static buffers. Maybe do flattening of accelerators with buffers already fixed.
#endif
#if 1
    TEST_INST( 1 ,SimpleMergeNoCommon);
    TEST_INST( 1 ,SimpleMergeUnitCommonNoEdge);
    TEST_INST( 1 ,SimpleMergeUnitAndEdgeCommon);
    TEST_INST( 1 ,SimpleMergeInputOutputCommon);
    TEST_INST( 0 ,ComplexMerge);
#endif
#endif

    //Free(versat);

    printf("\nAutomatic tests done (passed/total): %d / %d\n",info.testsPassed,info.numberTests);
}
