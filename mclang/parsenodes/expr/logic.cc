
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/logic.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

LogicNode::LogicNode(ParseNodeType type, ParseNode *left, ParseNode *right,
Loc loc) : ExprNode(type, loc, left, right) {

}

LogicNode::~LogicNode() {
    
}

void LogicNode::bytecode(BCManager &man) {
    if (type == PNODE_NOT)
        bytecodeNot(man);
    if (type == PNODE_AND)
        bytecodeAnd(man);
    if (type == PNODE_OR)
        bytecodeOr(man);
    MCLError(1, "Unexpected error occured when converting logical bytecode.",
    loc);
}

void LogicNode::invalidTypeError() const {
    if (type == PNODE_NOT)
        MCLError(1, "Operator \"!\" does not support operand of type \""
        + retLeft.type.str() + "\".", loc);
    std::string op = "";
    if (type == PNODE_AND)
        op = "&&";
    if (type == PNODE_OR)
        op = "||";
    MCLError(1, "Operator \"" + op + "\" does not support operands of types \""
    + retLeft.type.str() + "\" and \"" + retRight.type.str() + "\".");
}

void LogicNode::bytecodeNot(BCManager &man) {
    left->bytecode(man), retLeft = man.ret;
    if (!(man.ret.type >= Type("int")) && !(man.ret.type >= Type("bool")))
        invalidTypeError();
    if (man.ret.type.isConst) {
        man.ret = {Type("const bool"), std::stoi(man.ret.value) == 0 ? "1" :
        "0"};
    } else {
        man.write(BCInstr(INSTR_NOT, man.ret.value));
        man.ret = {Type("bool"), man.ret.value};
    }
}

void LogicNode::bytecodeAnd(BCManager &man) {
    left->bytecode(man), retLeft = man.ret;
    if (!(man.ret.type >= Type("int")) && !(man.ret.type >= Type("bool"))) {
        right->bytecode(man), retRight = man.ret;
        invalidTypeError();
    }
    std::string tmpVar = "";
    if (!retLeft.type.isConst) {
        tmpVar = man.tmp.reserve();
        man.write(BCInstr(INSTR_COPY, tmpVar, man.ret.value));
    }
    right->bytecode(man), retRight = man.ret;
    if (retLeft.type.isConst && retRight.type.isConst) {
        man.ret = {Type("const bool"), (retLeft.value == "0" || retRight.value
        == "0") ? "0" : "1"};
    } else if ((retLeft.type.isConst && retLeft.value == "0")
    || (retRight.type.isConst && retRight.value == "0")) {
        man.ret = {Type("const bool"), "0"};
    } else if (retLeft.type.isConst && retLeft.value != "0") {
        man.ret = {Type("bool"), man.ret.value};
    } else if (retRight.type.isConst && retRight.value != "0") {
        man.ret = {Type("bool"), tmpVar};
    } else {
        // Neither side is constant
        man.write(BCInstr(INSTR_AND, man.ret.value, tmpVar));
        man.ret = {Type("bool"), man.ret.value};
    }
    if (!retLeft.type.isConst)
        man.tmp.free(tmpVar);
}

void LogicNode::bytecodeOr(BCManager &man) {
    left->bytecode(man), retLeft = man.ret;
    if (!(man.ret.type >= Type("int")) && !(man.ret.type >= Type("bool"))) {
        right->bytecode(man), retRight = man.ret;
        invalidTypeError();
    }
    std::string tmpVar = "";
    if (!retLeft.type.isConst) {
        tmpVar = man.tmp.reserve();
        man.write(BCInstr(INSTR_COPY, tmpVar, man.ret.value));
    }
    right->bytecode(man), retRight = man.ret;
    if (retLeft.type.isConst && retRight.type.isConst) {
        man.ret = {Type("const bool"), (retLeft.value == "0" && retRight.value
        == "0") ? "0" : "1"};
    } else if ((retLeft.type.isConst && retLeft.value != "0")
    || (retRight.type.isConst && retRight.value != "0")) {
        man.ret = {Type("const bool"), "1"};
    } else if (retLeft.type.isConst && retLeft.value == "0") {
        man.ret = {Type("bool"), man.ret.value};
    } else if (retRight.type.isConst && retRight.value == "0") {
        man.ret = {Type("bool"), tmpVar};
    } else {
        // Neither side is constant
        man.write(BCInstr(INSTR_OR, man.ret.value, tmpVar));
        man.ret = {Type("bool"), man.ret.value};
    }
    if (!retLeft.type.isConst)
        man.tmp.free(tmpVar);
}