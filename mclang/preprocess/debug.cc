
#include "preprocess/debug.h"
#include "preprocess/preptoken.h"
#include <string>
#include <vector>

std::string prepDebugTable(std::vector<PrepToken> *toks) {
    std::string out = "";
    for (unsigned int i = 0; i < toks->size(); i++) {
        PrepToken cur = (*toks)[i];
        std::string tname = prepTokenNames[(unsigned int)(cur.type)];
        while (tname.size() < 10)
            tname.push_back(' ');
        std::string locInfo = std::to_string(cur.loc.line) + "["
        + std::to_string(cur.loc.col) + "]";
        while (locInfo.size() < 10)
            locInfo.push_back(' ');
        out += "Type: " + tname + " Line: " + locInfo + " Content: "
        + cur.content;
        if (i < toks->size() - 1)
            out.push_back('\n');
    }
    return out;
}