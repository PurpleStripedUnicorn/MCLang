
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
        for (unsigned int j = 0; j < (*data)[i]->instrList.size(); j++) {
            BCInstr curInstr = (*data)[i]->instrList[j];
            if (curInstr.type == INSTR_CMD) {
                cur.cmdList.push_back(convertCmd(curInstr));
            } else if (curInstr.type == INSTR_EXEC_CALL) {
                cur.cmdList.push_back(convertExecCall(curInstr));
            } else {
                MCLError(1, "Invalid instruction being read. Unexpected error."
                );
            }
        }
        out.push_back(cur);
    }
    return out;
}

std::string BCConverter::convertCmd(BCInstr instr) const {
    return instr.arg1;
}

std::string BCConverter::convertExecCall(BCInstr instr) const {
    return "execute " + instr.arg1 + " run function " + comp->ns + ":"
    + instr.arg2;
}