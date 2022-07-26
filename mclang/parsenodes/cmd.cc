
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
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
    if (cmd.substr(0, 9) == "function ")
        MCLError(0, "Raw function call insertion has undefined behaviour, use "
        "normal function call instead!", loc);
    man.write(BCInstr(INSTR_CMD, cmd));
    man.ret.type = Type("void");
    man.ret.value = "";
}