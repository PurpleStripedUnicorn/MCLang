
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

AssignNode::AssignNode(std::string varName, ParseNode *expr, Loc loc) :
ExprNode(PNODE_ASSIGN, loc, expr), varName(varName) {

}

AssignNode::~AssignNode() {
    
}

void AssignNode::bytecode(BCManager &man) {
    left->bytecode(man);
    Type varType;
    if (!man.ctx.findVarAll(varName, varType))
        MCLError(1, "Use of uninitialized variable \"" + varName + "\".", loc);
    if (man.ret.type != varType)
        MCLError(1, "Cannot assign value of type \"" + man.ret.type.str()
        + "\" to variable of type \"" + varType.str() + "\".", loc);
    man.write(BCInstr(INSTR_COPY, varName, man.ret.value));
}

std::string AssignNode::getVarName() const {
    return varName;
}

ParseNode *AssignNode::getExpr() const {
    return left;
}