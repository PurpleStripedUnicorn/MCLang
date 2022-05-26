
#include "parsenodes/cmd.h"

CmdNode::CmdNode(std::string cmd) : ParseNode(PNODE_CMD), cmd(cmd) {

}

CmdNode::~CmdNode() {
    
}

std::vector<ParseNode *> CmdNode::children() const {
    return {};
}

void CmdNode::bytecode(BCManager &man) const {
    man.write(BCInstr(INSTR_CMD, cmd));
}