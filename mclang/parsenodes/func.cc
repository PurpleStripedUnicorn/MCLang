
#include "bcgen/bcgen.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

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