
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/compare.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CompareNode::CompareNode(ParseNodeType type, ParseNode *left, ParseNode *right,
Loc loc) : ExprNode(type, loc, left, right) {

}

CompareNode::~CompareNode() {
    
}

void CompareNode::bytecode(BCManager &man) {
    left->bytecode(man), retLeft = man.ret;
    if (!(man.ret.type >= Type("int"))) {
        right->bytecode(man), retRight = man.ret;
        invalidTypeError();
    }
    std::string tmpVarLeft = "", tmpVarRight = "";
    std::string leftVar = retLeft.value;
    if (retLeft.type.isConst) {
        tmpVarLeft = man.tmp.reserve();
        man.write(BCInstr(INSTR_SET, tmpVarLeft, retLeft.value));
        leftVar = tmpVarLeft;
    }
    right->bytecode(man), retRight = man.ret;
    std::string rightVar = retRight.value;
    if (retRight.type.isConst) {
        tmpVarRight = man.tmp.reserve();
        man.write(BCInstr(INSTR_SET, tmpVarRight, retRight.value));
        rightVar = tmpVarRight;
    }
    if (tmpVarLeft != "")
        man.tmp.free(tmpVarLeft);
    if (tmpVarRight != "")
        man.tmp.free(tmpVarRight);
    man.write(BCInstr(getInstrType(), leftVar, rightVar));
    man.ret = {Type("bool"), leftVar};
}

void CompareNode::invalidTypeError() const {
    std::string op = "";
    if (type == PNODE_LT)
        op = "<";
    if (type == PNODE_LTE)
        op = "<=";
    if (type == PNODE_GT)
        op = ">";
    if (type == PNODE_GTE)
        op = ">=";
    if (type == PNODE_EQ)
        op = "==";
    if (type == PNODE_NEQ)
        op = "!=";
    MCLError(1, "Operator \"" + op + "\" does not support operands of types \""
    + retLeft.type.str() + "\" and \"" + retRight.type.str() + "\".");
}

BCInstrType CompareNode::getInstrType() const {
    if (type == PNODE_LT)
        return INSTR_LT;
    if (type == PNODE_LTE)
        return INSTR_LTE;
    if (type == PNODE_GT)
        return INSTR_GT;
    if (type == PNODE_GTE)
        return INSTR_GTE;
    if (type == PNODE_EQ)
        return INSTR_EQ;
    if (type == PNODE_NEQ)
        return INSTR_NEQ;
    MCLError(1, "Unexpected error occured while converting comparison "
    "node bytecode.", loc);
    return INSTR_ERR;
}