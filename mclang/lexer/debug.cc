
#include "lexer/debug.h"

std::string lexerDebugTable(std::vector<Token> toks) {
    std::string out = "";
    for (unsigned int i = 0; i < toks.size(); i++) {
        Token cur = toks[i];
        std::string tname = tokenTypeNames[(unsigned int)(cur.type)];
        while (tname.size() < 10)
            tname.push_back(' ');
        out += "Type: " + tname + " Content: " + cur.content;
        if (i < toks.size() - 1)
            out.push_back('\n');
    }
    return out;
}