
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/num.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

NumNode::NumNode(std::string content, Loc loc) : ParseNode(PNODE_NUM, loc),
content(content) {

}

NumNode::~NumNode() {

}

std::vector<ParseNode *> NumNode::children() const {
    return {};
}

void NumNode::bytecode(BCManager &man) {
    man.ret = {Type("const int"), content};
}

std::string NumNode::getContent() const {
    return content;
}