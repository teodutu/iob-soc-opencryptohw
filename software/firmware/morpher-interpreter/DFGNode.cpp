#include "DFGNode.h"

static pugi::xml_node GeXMLDFG(const char* xml_file)
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

static Instruction CreateInstruction(const char* instruction_str)
{
    if (!strcmp(instruction_str, "ADD"))
        return Instruction::ADD;
    else if (!strcmp(instruction_str, "SUB"))
        return Instruction::SUB;
    else if (!strcmp(instruction_str, "MUL"))
        return Instruction::MUL;
    else if (!strcmp(instruction_str, "DIV"))
        return Instruction::DIV;
    else if (!strcmp(instruction_str, "SHL"))
        return Instruction::SHL;
    else if (!strcmp(instruction_str, "SHR"))
        return Instruction::SHR;
    else if (!strcmp(instruction_str, "LOAD"))
        return Instruction::LOAD;
    else if (!strcmp(instruction_str, "OLOAD"))
        return Instruction::OLOAD;
    else if (!strcmp(instruction_str, "STORE"))
        return Instruction::STORE;
    else if (!strcmp(instruction_str, "OSTORE"))
        return Instruction::OSTORE;
    else if (!strcmp(instruction_str, "LOADB"))
        return Instruction::LOADB;
    else if (!strcmp(instruction_str, "STOREB"))
        return Instruction::STOREB;
    else if (!strcmp(instruction_str, "LS"))
        return Instruction::LS;
    else if (!strcmp(instruction_str, "CMERGE"))
        return Instruction::CMERGE;
    else if (!strcmp(instruction_str, "CGT"))
        return Instruction::CGT;
    else if (!strcmp(instruction_str, "MOVC"))
        return Instruction::MOVC;
    else if (!strcmp(instruction_str, "CMP"))
        return Instruction::CMP;
    else if (!strcmp(instruction_str, "SELECT"))
        return Instruction::SELECT;
    else {
        std::cerr << "Invalid instruction: " << instruction_str << '\n';
        assert(false);
    }
}

std::unordered_map<size_t, DFGNode> ReadDFG(const char* xml_file)
{
    std::unordered_map<size_t, DFGNode> dfg;
    pugi::xml_node pugi_dfg = GeXMLDFG(xml_file);
    std::string var_name;
    size_t const_val;
    
    if (!pugi_dfg) {
        std::cerr << "Could not parse XML file " << xml_file << '\n';
        return dfg;
    }

    for (pugi::xml_node node = pugi_dfg.child("Node"); node; node = node.next_sibling("Node")) {
        DFGNode instr_node;
        instr_node.idx = node.attribute("idx").as_ullong();

        instr_node.instr = CreateInstruction(node.child_value("OP"));
        instr_node.var = node.child_value("BasePointerName");
        instr_node.bb = node.child_value("BB");
        instr_node.asap = node.attribute("ASAP").as_int();
        instr_node.alap = node.attribute("ALAP").as_int();

        pugi::xml_attribute const_attr = node.attribute("CONST");
        if (const_attr)
            instr_node.const_val = const_attr.as_int();
        else
            instr_node.const_val = -1;

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

    // std::vector<DFGNode> nodes;
    // for (auto& node : dfg)
    //     nodes.emplace_back(node.second);

    // std::sort(nodes.begin(), nodes.end(), [](const DFGNode& a, const DFGNode& b) {
    //     if (a.asap == b.asap)
    //         return a.alap < b.alap;
    //     else
    //         return a.asap < b.asap;
    // });

    // for (auto& node : nodes)
    //     std::cerr << "Node " << node.idx << ": asap = " << node.asap << ", alap = " << node.alap << '\n';

    return dfg;
}
