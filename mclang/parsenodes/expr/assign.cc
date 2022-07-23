
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
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

void AssignNode::bytecode(BCManager &man) const {
    // Puts the result in "__res" (which is already done by evaluating the
    // expression at the rhs)
    // Check if the variable is initialized
    if (!man.varManager.hasVar(varName))
        MCLError(1, "Use of uninitialized variable \"" + varName + "\"", loc);
    left->bytecode(man);
    man.write(BCInstr(INSTR_COPY, varName, "__res"));
}

std::string AssignNode::getVarName() const {
    return varName;
}