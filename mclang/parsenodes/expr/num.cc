
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "parsenodes/expr/num.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

NumNode::NumNode(std::string content, ParseNodeProps props) :
ParseNode(PNODE_NUM, props), content(content) {

}

NumNode::~NumNode() {

}

std::vector<ParseNode *> NumNode::children() const {
    return {};
}

void NumNode::bytecode(BCManager &man) const {
    // Put the value in "__res"
    man.write(BCInstr(INSTR_SET, "__res", content));
}

std::string NumNode::getContent() const {
    return content;
}