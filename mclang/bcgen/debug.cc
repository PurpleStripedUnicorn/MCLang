
#include "bcgen/debug.h"

std::string bcgenInstrList(const std::vector<BCFunc> &instrList) {
    std::string out = "";
    for (unsigned int i = 0; i < instrList.size(); i++) {
        const BCFunc *cur = &instrList[i];
        out.append(cur->name + ":");
        for (unsigned int j = 0; j < cur->instrList.size(); j++) {
            BCInstr curInstr = cur->instrList[j];
            std::string curLine = "";
            curLine.append("\n  " + instrNames[(unsigned int)curInstr.type]);
            while (curLine.size() < 10)
                curLine.push_back(' ');
            if (curInstr.arg1 != "")
                curLine.append(curInstr.arg1);
            if (curInstr.arg2 != "")
                curLine.append(", " + curInstr.arg2);
            out.append(curLine);
        }
        if (i < instrList.size() - 1)
            out.append("\n\n");
    }
    return out;
}