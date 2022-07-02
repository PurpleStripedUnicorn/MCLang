
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

NSNode::NSNode(std::string fname, ParseNodeProps props) :
ParseNode(PNODE_CALL, props), nsName(nsName) {

}

NSNode::~NSNode() {
    
}

std::vector<ParseNode *> NSNode::children() const {
    return {};
}

void NSNode::bytecode(BCManager &man) const {
    // No bytecode is generated, because this node serves as a global setting,
    // which is handled by the "program" parse node instead
}