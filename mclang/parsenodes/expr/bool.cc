
#include "bcgen/bcgen.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/bool.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

BoolNode::BoolNode(std::string content, Loc loc) : ParseNode(PNODE_NUM, loc),
content(content) {

}

BoolNode::~BoolNode() {

}

std::vector<ParseNode *> BoolNode::children() const {
    return {};
}

void BoolNode::bytecode(BCManager &man) {
    man.ret = {Type("const bool"), content};
}

std::string BoolNode::getContent() const {
    return content;
}