
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

NSNode::NSNode(std::string nsName, Loc loc) :
ParseNode(PNODE_NAMESPACE, loc), nsName(nsName) {

}

NSNode::~NSNode() {
    
}

std::vector<ParseNode *> NSNode::children() const {
    return {};
}

void NSNode::bytecode(BCManager &man) const {
    // No bytecode is generated, because this node serves as a global setting,
    // which is handled by the "program" parse node instead
    // Prevent unused parameter warning:
    (void)man;
}

std::string NSNode::getName() const {
    return nsName;
}