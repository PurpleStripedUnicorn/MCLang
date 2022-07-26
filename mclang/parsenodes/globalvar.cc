
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/globalvar.h"
#include "parsenodes/word.h"
#include <string>

GlobalVarNode::GlobalVarNode(std::string varType, std::string varName, Loc loc)
: ParseNode(PNODE_GLOBALVAR, loc), varType(varType),
varName(varName) {

}

GlobalVarNode::~GlobalVarNode() {

}

std::vector<ParseNode *> GlobalVarNode::children() const {
    return {};
}

void GlobalVarNode::bytecode(BCManager &man) const {
    // Check if the global variable name isn't already in use
    Type tmp;
    if (man.ctx.findVarAll(varName, tmp))
        MCLError(1, "Global variable \"" + varName + "\" already defined", loc);
    man.ctx.addVar(Var(Type(varType), varName));
    man.ret.type = Type("void");
    man.ret.value = "";
}