
#include "parsenodes/cmd.h"

CmdNode::CmdNode(std::string cmd) : ParseNode(PNODE_CMD), cmd(cmd) {

}

CmdNode::~CmdNode() {
    
}

std::vector<ParseNode *> CmdNode::children() const {
    return {};
}