
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/varinit.h"
#include "parsenodes/word.h"
#include <string>

VarInitNode::VarInitNode(Type varType, ParseNode *childExpr, Loc loc) :
ParseNode(PNODE_VARINIT, loc), varType(varType), childExpr(childExpr) {

}

VarInitNode::~VarInitNode() {

}

std::vector<ParseNode *> VarInitNode::children() const {
    return {childExpr};
}

void VarInitNode::bytecode(BCManager &man) const {
    // Check if the type of the child expression is correct and get the variable
    // name
    std::string varName;
    if (childExpr->getType() == PNODE_ASSIGN)
        varName = ((AssignNode *)childExpr)->getVarName();
    else if (childExpr->getType() == PNODE_WORD)
        varName = ((WordNode *)childExpr)->getContent();
    else
        MCLError(1, "Invalid expression after initialization.", loc);
    // Check if the name of the variable is already in use
    Type tmp;
    if (man.ctx.findVarAll(varName, tmp))
        MCLError(1, "Initialization of already defined variable \"" + varName
        + "\"", loc);
    // Register variable with type
    man.ctx.addVar(Var(varType, varName));
    if (childExpr->getType() == PNODE_ASSIGN)
        childExpr->bytecode(man);
    man.ret.type = Type("void");
    man.ret.value = "";
}