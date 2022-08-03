
#include "bcconvert/bcconvert.h"
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include <string>
#include <vector>

CmdFunc::CmdFunc(std::string name) : name(name) {

}

BCConverter::BCConverter(Compiler *comp) : comp(comp), data(nullptr) {

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
    if (instr.type == INSTR_COPY)
        return {"scoreboard players operation " + instr.arg1 + " " +
        comp->scoreboardName + " = " + instr.arg2 + " " + comp->scoreboardName};
    if (INSTR_ADD <= instr.type && instr.type <= INSTR_MOD)
        return convertArith(instr);
    if (INSTR_ADDI <= instr.type && instr.type <= INSTR_MODI)
        return convertArithI(instr);
    if (INSTR_LT <= instr.type && instr.type <= INSTR_NEQ)
        return convertComparison(instr);
    if (INSTR_NOT <= instr.type && instr.type <= INSTR_OR)
        return convertLogical(instr);
    MCLError(1, "Unexpected error, reading undefined instruction");
    return {};
}

std::vector<std::string> BCConverter::convertCmd(BCInstr instr) const {
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

std::vector<std::string> BCConverter::convertArith(BCInstr instr) const {
    std::string op = "";
    if (instr.type == INSTR_ADD)
        op = "+=";
    if (instr.type == INSTR_SUB)
        op = "-=";
    if (instr.type == INSTR_MUL)
        op = "*=";
    if (instr.type == INSTR_DIV)
        op = "/=";
    if (instr.type == INSTR_MOD)
        op = "%=";
    return {"scoreboard players operation " + instr.arg1 + " "
    + comp->scoreboardName + " " + op + " " + instr.arg2 + " "
    + comp->scoreboardName};
}

std::vector<std::string> BCConverter::convertArithI(BCInstr instr) const {
    // ADDI and SUBI can be done in just one command
    if (instr.type == INSTR_ADDI)
        return {"scoreboard players add " + instr.arg1 + " " +
        comp->scoreboardName + " " + instr.arg2};
    if (instr.type == INSTR_SUBI)
        return {"scoreboard players remove " + instr.arg1 + " " +
        comp->scoreboardName + " " + instr.arg2};
    // For others the numbers are first put in temporary scoreboard values
    std::string op = "";
    if (instr.type == INSTR_MULI)
        op = "*=";
    if (instr.type == INSTR_DIVI)
        op = "/=";
    if (instr.type == INSTR_MODI)
        op = "%=";
    return {"scoreboard players set 0itmp " + comp->scoreboardName + " "
    + instr.arg2, "scoreboard players operation " + instr.arg1 + " "
    + comp->scoreboardName + " " + op + " 0itmp " + comp->scoreboardName};
}

std::vector<std::string> BCConverter::convertComparison(BCInstr instr) const {
    std::string sb = comp->scoreboardName;
    std::string op = "";
    if (instr.type == INSTR_LT)
        op = "<";
    if (instr.type == INSTR_LTE)
        op = "<=";
    if (instr.type == INSTR_GT)
        op = ">";
    if (instr.type == INSTR_GTE)
        op = ">=";
    if (instr.type == INSTR_EQ)
        op = "=";
    if (op == "")
        MCLError(1, "Unexpected error while converting comparison instruction."
        );
    return {"scoreboard players set 0itmp" + sb + " 0",
    "execute if score " + instr.arg1 + " " + sb + " " + op + " " + instr.arg2
    + " " + sb + " run scoreboard players set 0itmp " + sb + " 1",
    "scoreboard players operation " + instr.arg1 + " " + sb + " = 0itmp " + sb};
}

std::vector<std::string> BCConverter::convertLogical(BCInstr instr) const {
    std::string sb = comp->scoreboardName;
    if (instr.type == INSTR_NOT)
        return {"scoreboard players set 0itmp" + sb + " 0",
        "execute if score " + instr.arg1 + " " + sb + " matches 0 run "
        "scoreboard players set 0itmp " + sb + " 1", "scoreboard players "
        "operation " + instr.arg1 + " " + sb + " = 0itmp " + sb};
    if (instr.type == INSTR_AND)
        return {"execute unless score " + instr.arg1 + " " + sb + " matches 0 "
        "run scoreboard players set " + instr.arg1 + " " + sb + " 1",
        "execute if score " + instr.arg2 + " " + sb + " matches 0 run "
        "scoreboard players set " + instr.arg1 + " " + sb + " 0"};
    if (instr.type == INSTR_OR)
        return {"execute unless score " + instr.arg1 + " " + sb + " matches 0 "
        "run scoreboard players set " + instr.arg1 + " " + sb + " 1",
        "execute unless score " + instr.arg2 + " " + sb + " matches 0 run "
        "scoreboard players set " + instr.arg1 + " " + sb + " 1"};
    MCLError(1, "Unexpected error while converting logical instruction.");
    return {};
}