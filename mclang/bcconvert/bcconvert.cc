
#include "bcconvert/bcconvert.h"

CmdFunc::CmdFunc(std::string name) : name(name) {

}

BCConverter::BCConverter(std::vector<BCFunc> *data) : data(data) {

}

BCConverter::~BCConverter() {

}

std::vector<CmdFunc> BCConverter::getRawCommands() {
    std::vector<CmdFunc> out;
    for (unsigned int i = 0; i < data->size(); i++) {
        CmdFunc cur((*data)[i].name);
        for (unsigned int j = 0; j < (*data)[i].instrList.size(); j++) {
            BCInstr curInstr = (*data)[i].instrList[j];
            if (curInstr.type == INSTR_CMD) {
                cur.cmdList.push_back(convertCmd(curInstr));
            } else {
                MCLError(1, "Invalid instruction being read. Unexpected error.",
                0, 0);
            }
        }
        out.push_back(cur);
    }
    return out;
}

std::string BCConverter::convertCmd(BCInstr instr) const {
    return instr.arg1;
}