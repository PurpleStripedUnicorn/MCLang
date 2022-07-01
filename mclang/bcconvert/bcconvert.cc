
#include "bcconvert/bcconvert.h"
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include <string>
#include <vector>

CmdFunc::CmdFunc(std::string name) : name(name) {

}

BCConverter::BCConverter(Compiler *comp) : comp(comp), data(NULL) {

}

BCConverter::~BCConverter() {

}

std::vector<CmdFunc> BCConverter::getRawCommands() {
    data = (std::vector<BCFunc *> *)comp->bcMan->getBytecode();
    std::vector<CmdFunc> out;
    for (unsigned int i = 0; i < data->size(); i++) {
        CmdFunc cur((*data)[i]->name);
        cur.cmdList = convertFunc(*(*data)[i]);
        out.push_back(cur);
    }
    return out;
}

std::vector<std::string> BCConverter::convertFunc(const BCFunc &func) {
    // Check for stack operations, if these exist the stack will have to be
    // created
    bool hasStackOp = false;
    std::vector<std::string> out;
    for (unsigned int i = 0; i < func.instrList.size(); i++) {
        BCInstr instr = func.instrList[i];
        if (instr.type == INSTR_PUSH || instr.type == INSTR_POP
        || instr.type == INSTR_TOP)
            hasStackOp = true;
        std::vector<std::string> list = convertInstr(instr);
        out.insert(out.end(), list.begin(), list.end());
    }
    if (hasStackOp)
        out.insert(out.begin(), STACK_CREATE);
    // Create scoreboard objective to store variables, if it doesn't exist
    // already
    out.insert(out.begin(), "scoreboard objectives add " + comp->scoreboardName
    + " dummy");
    return out;
}

std::vector<std::string> BCConverter::convertInstr(const BCInstr &instr) {
    if (instr.type == INSTR_CMD)
        return convertCmd(instr);
    if (instr.type == INSTR_EXEC_CALL)
        return convertExecCall(instr);
    if (instr.type == INSTR_CALL)
        return {"function " + comp->ns + ":" + instr.arg1};
    if (instr.type == INSTR_SET)
        return convertSet(instr);
    if (instr.type == INSTR_PUSH || instr.type == INSTR_POP
    || instr.type == INSTR_TOP)
        return convertStackOp(instr);
    MCLError(1, "Unexpected error, reading undefined instruction");
    return {};
}

std::vector<std::string> BCConverter::convertCmd(BCInstr instr) const {
    if (instr.arg1.rfind("function ", 0) == 0)
        MCLError(0, "Raw function call insertion has undefined behaviour, use "
        "a function call instead!");
    return {instr.arg1};
}

std::vector<std::string> BCConverter::convertExecCall(BCInstr instr) const {
    return {"execute " + instr.arg1 + " run function " + comp->ns + ":"
    + instr.arg2};
}

std::vector<std::string> BCConverter::convertSet(BCInstr instr) const {
    return {"scoreboard players set " + instr.arg1 + " " + comp->scoreboardName
    + " " + instr.arg2};
}

std::vector<std::string> BCConverter::convertStackOp(BCInstr instr) const {
    std::string param = instr.arg1;
    if (instr.type == INSTR_TOP)
        return {"execute store result score " + param + " "
        + comp->scoreboardName + " run data get storage mclang:stack stack[0]"};
    if (instr.type == INSTR_PUSH)
        return {
            "data modify storage mclang:stack stack prepend value 0",
            "execute store result storage mclang:stack stack[0] int 1 run "
            "scoreboard players get " + param + " " + comp->scoreboardName
        };
    if (instr.type == INSTR_POP)
        return {"data remove storage mclang:stack stack[0]"};
    MCLError(1, "Unexpected error, reading undefined stack operation "
    "instruction");
    return {};
}