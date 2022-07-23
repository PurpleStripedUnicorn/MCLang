
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/cmd.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CmdNode::CmdNode(std::string cmd, Loc loc) : ParseNode(PNODE_CMD, loc), cmd(cmd)
{

}

CmdNode::~CmdNode() {
    
}

std::vector<ParseNode *> CmdNode::children() const {
    return {};
}

void CmdNode::bytecode(BCManager &man) const {
    man.write(BCInstr(INSTR_CMD, cmd));
}