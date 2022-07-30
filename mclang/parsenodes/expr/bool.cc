
#include "bcgen/bcgen.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/bool.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

BoolNode::BoolNode(bool value, Loc loc) : ParseNode(PNODE_NUM, loc),
value(value) {

}

BoolNode::~BoolNode() {

}

std::vector<ParseNode *> BoolNode::children() const {
    return {};
}

void BoolNode::bytecode(BCManager &man) {
    man.ret = {Type("const bool"), value ? "0" : "1"};
}

bool BoolNode::getValue() const {
    return value;
}