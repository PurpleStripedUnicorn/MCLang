
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
    // This also retrieves the type of the variable
    if (!man.ctx.findVarAll(varName, varType))
        MCLError(1, "Use of uninitialized variable \"" + varName + "\".", loc);
    if (varType.isConst)
        MCLError(1, "Cannot modify a constant variable.", loc);
    if (varType == Type("int") || varType == Type("bool")) {
        left->bytecode(man);
        if (!man.ret.type.sameBase(varType))
            invalidTypeError(man);
        if (man.ret.type.isConst) {
            Var tmpVar = man.ctx.makeUniqueVar(varType);
            man.write(BCInstr(INSTR_SET, varName, man.ret.value));
            man.write(BCInstr(INSTR_SET, tmpVar.name, man.ret.value));
        } else {
            man.write(BCInstr(INSTR_COPY, varName, man.ret.value));
        }
    } else {
        invalidTypeError(man);
    }
}

std::string AssignNode::getVarName() const {
    return varName;
}

ParseNode *AssignNode::getExpr() const {
    return left;
}

void AssignNode::invalidTypeError(BCManager &man) const {
    MCLError(1, "Cannot assign value of type \"" + man.ret.type.str()
    + "\" to variable of type \"" + varType.str() + "\".", loc);
}