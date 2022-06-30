
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/call.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CallNode::CallNode(std::string fname, ParseNodeProps props) :
ParseNode(PNODE_CALL, props), fname(fname) {

}

CallNode::~CallNode() {
    
}

std::vector<ParseNode *> CallNode::children() const {
    return {};
}

void CallNode::bytecode(BCManager &man) const {
    // TODO: implement bytecode generation
}