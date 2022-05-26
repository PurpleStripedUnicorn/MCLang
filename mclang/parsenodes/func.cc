
#include "parsenodes/func.h"

FuncNode::FuncNode(std::string name, CodeBlockNode *codeblock,
ParseNodeProps props) : ParseNode(PNODE_FUNC, props), name(name),
codeblock(codeblock) {

}

std::vector<ParseNode *> FuncNode::children() const {
    return {(ParseNode *)codeblock};
}

FuncNode::~FuncNode() {
    delete codeblock;
}

void FuncNode::bytecode(BCManager &man) const {
    man.addFunc(name);
    codeblock->bytecode(man);
    man.popFunc();
}