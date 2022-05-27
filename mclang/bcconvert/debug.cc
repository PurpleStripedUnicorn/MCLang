
#include "bcconvert/debug.h"

std::string bcconvertCmdList(const std::vector<CmdFunc> &funcList) {
    std::string out = "";
    for (unsigned int i = 0; i < funcList.size(); i++) {
        const CmdFunc *cur = &funcList[i];
        out.append(cur->name + ":");
        for (unsigned int j = 0; j < cur->cmdList.size(); j++)
            out.append("\n    " + cur->cmdList[j]);
        if (i < funcList.size() - 1)
            out.append("\n\n");
    }
    return out;
}