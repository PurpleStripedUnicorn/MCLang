
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
    if (!isInitialized(man, varType))
        MCLError(1, "Use of uninitialized variable \"" + varName + "\".", loc);
    if (varType.isConst)
        MCLError(1, "Cannot modify a constant variable.", loc);
    std::string tmpVar = man.tmp.reserve();
    if (varType == Type("int") || varType == Type("bool")) {
        left->bytecode(man);
        if (!man.ret.type.sameBase(varType))
            invalidTypeError(man);
        if (man.ret.type.isConst) {
            man.write(BCInstr(INSTR_SET, varName, man.ret.value));
            man.write(BCInstr(INSTR_SET, tmpVar, man.ret.value));
        } else {
            man.write(BCInstr(INSTR_COPY, varName, man.ret.value));
        }
    } else {
        invalidTypeError(man);
    }
    man.tmp.free(tmpVar);
    man.ret = {man.ret.type, man.ret.value};
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

bool AssignNode::isInitialized(BCManager &man, Type &varType) const {
    for (const Context &ctx : man.ctx)
        for (const Var &var : ctx.vars)
            if (var.name == varName)
                return true;
    return false;
}