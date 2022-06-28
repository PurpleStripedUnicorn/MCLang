
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/if.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

IfNode::IfNode(std::vector<std::string> args, std::vector<CodeBlockNode *>
codeblocks, ParseNodeProps props) : ParseNode(PNODE_IF, props), ifArgs(args),
codeblocks(codeblocks) {

}

std::vector<ParseNode *> IfNode::children() const {
    std::vector<ParseNode *> out;
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        out.push_back(codeblocks[i]);
    return out;
}

IfNode::~IfNode() {
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        delete codeblocks[i];
}

void IfNode::bytecode(BCManager &man) const {
    // TODO: implement
}

bool IfNode::hasElse() const {
    return ifArgs.size() < codeblocks.size();
}