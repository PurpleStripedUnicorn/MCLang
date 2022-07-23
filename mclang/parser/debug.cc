
#include "parsenodes/parsenode.h"
#include "parser/debug.h"
#include <string>

std::string parserDebugTree(ParseNode *root, unsigned int pad) {
    std::string out = "";
    out.append("Line: " + std::to_string(root->getLoc().line) + "["
    + std::to_string(root->getLoc().col) + "]    ");
    for (unsigned int i = 0; i < pad; i++)
        out.append("  ");
    out += parseNodeNames[(unsigned int)root->getType()];
    std::vector<ParseNode *> children = root->children();
    for (unsigned int i = 0; i < children.size(); i++) {
        out.push_back('\n');
        out.append(parserDebugTree(children[i], pad + 1));
    }
    return out;
}
