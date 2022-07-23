
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/globalvar.h"
#include "parsenodes/word.h"
#include <string>

GlobalVarNode::GlobalVarNode(std::string varType, std::string varName,
ParseNodeProps props) : ParseNode(PNODE_GLOBALVAR, props), varType(varType),
varName(varName) {

}

GlobalVarNode::~GlobalVarNode() {

}

std::vector<ParseNode *> GlobalVarNode::children() const {
    return {};
}

void GlobalVarNode::bytecode(BCManager &man) const {
    // TODO: Implement different variable types
    // Check if the global variable name isn't already in use
    if (man.varManager.hasVar(varName))
        MCLError(1, "Global variable \"" + varName + "\" already defined",
        props.loc.line, props.loc.col);
    man.varManager.addVar(varName);
}